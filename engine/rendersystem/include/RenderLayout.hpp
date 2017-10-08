#ifndef _Air_Render_Layout_H_
#define _Air_Render_Layout_H_
#pragma once
#include "PreDeclare.h"
#include "rendersystem/include/GraphicsBuffer.hpp"
#include "ElementFormat.h"
namespace Air
{
	enum VertexElementUsage
	{
		VEU_Position = 0,
		VEU_Normal,
		VEU_Diffuse,
		VEU_Specular,
		VEU_BlendWeight,
		VEU_BlendIndex,
		VEU_TextureCoord,
		VEU_Tangent,
		VEU_Binormal
	};

	struct VertexElement
	{
		VertexElement()
		{

		}
		VertexElement(VertexElementUsage usage, uint8_t usage_index, ElementFormat format)
			:mUsage(usage), mUsageIndex(usage_index), mFormat(format)
		{

		}

		uint16_t getElementSize() const
		{
			return getNumFormatBytes(mFormat);
		}
		friend bool
			operator == (VertexElement const &lhs, VertexElement const & rhs)
		{
			return (lhs.mUsage == rhs.mUsage)
				&& (lhs.mUsageIndex == rhs.mUsageIndex)
				&& (lhs.mFormat == rhs.mFormat);
		}
		VertexElementUsage mUsage;
		uint8_t mUsageIndex;
		ElementFormat mFormat;
	};
	typedef std::vector<VertexElement> VertexElementsType;

	class AIR_CORE_API RenderLayout
	{
	public:
		enum TopologyType
		{
			TT_PointList,
			TT_LineList,
			TT_LineStrip,
			TT_TriangleList,
			TT_TriangleStrip,
			TT_LineList_Adj,
			TT_LineStrip_Adj,
			TT_TriangleList_Adj,
			TT_TriangleStrip_Adj,
			TT_1_Ctrl_Pt_PatchList,
			TT_2_Ctrl_Pt_PatchList,
			TT_3_Ctrl_Pt_PatchList,
			TT_4_Ctrl_Pt_PatchList,
			TT_5_Ctrl_Pt_PatchList,
			TT_6_Ctrl_Pt_PatchList,
			TT_7_Ctrl_Pt_PatchList,
			TT_8_Ctrl_Pt_PatchList,
			TT_9_Ctrl_Pt_PatchList,
			TT_10_Ctrl_Pt_PatchList,
			TT_11_Ctrl_Pt_PatchList,
			TT_12_Ctrl_Pt_PatchList,
			TT_13_Ctrl_Pt_PatchList,
			TT_14_Ctrl_Pt_PatchList,
			TT_15_Ctrl_Pt_PatchList,
			TT_16_Ctrl_Pt_PatchList,
			TT_17_Ctrl_Pt_PatchList,
			TT_18_Ctrl_Pt_PatchList,
			TT_19_Ctrl_Pt_PatchList,
			TT_20_Ctrl_Pt_PatchList,
			TT_21_Ctrl_Pt_PatchList,
			TT_22_Ctrl_Pt_PatchList,
			TT_23_Ctrl_Pt_PatchList,
			TT_24_Ctrl_Pt_PatchList,
			TT_25_Ctrl_Pt_PatchList,
			TT_26_Ctrl_Pt_PatchList,
			TT_27_Ctrl_Pt_PatchList,
			TT_28_Ctrl_Pt_PatchList,
			TT_29_Ctrl_Pt_PatchList,
			TT_30_Ctrl_Pt_PatchList,
			TT_31_Ctrl_Pt_PatchList,
			TT_32_Ctrl_Pt_PatchList,
			TT_UNKNOWN
		};

		enum StreamType
		{
			ST_Geometry,
			ST_Instance
		};

		RenderLayout()
		{
			mVertexStreams.reserve(4);
		}
		virtual ~RenderLayout() = 0;

		void setTopologyType(TopologyType type)
		{
			mTopologyType = type;
		}

		TopologyType getTopologyType() const {
			return mTopologyType;
		}

		GraphicsBufferPtr const & getVertexStream(uint32_t index) const
		{
			return mVertexStreams[index].mStream;
		}

		VertexElementsType const & getVertexStreamFormat(uint32_t index) const
		{
			return mVertexStreams[index].mFormat;
		}

		void setVertexStream(uint32_t index, GraphicsBufferPtr const & gb)
		{
			mVertexStreams[index].mStream = gb;
			mStreamsDirty = true;
		}

		GraphicsBufferPtr const & getIndexStream() const
		{
			BOOST_ASSERT(mIndexStream);
			return mIndexStream;
		}

		ElementFormat getIndexStreamFormat() const
		{
			return mIndexFormat;
		}

		uint32_t getNumVertexStreams() const
		{
			return static_cast<uint32_t>(mVertexStreams.size());
		}

		void setNumVertices(uint32_t n)
		{
			mForceNumVertices = n;
			mStreamsDirty = true;
		}
		uint32_t getNumVertices() const 
		{
			uint32_t n;
			if (0xffffffff == mForceNumVertices)
			{
				n = mVertexStreams[0].mStream->getSize() / mVertexStreams[0].mVertexSize;
			}
			else
			{
				n = mForceNumVertices;
			}
			return n;
		}
		uint32_t getVertexSize(uint32_t index) const
		{
			return mVertexStreams[index].mVertexSize;
		}

		std::vector<VertexElement> const & getInstanceStreamFormat() const
		{
			return mInstanceStream.mFormat;
		}

		uint32_t getInstanceSize() const
		{
			return mInstanceStream.mVertexSize;
		}

		void setNumIndices(uint32_t n)
		{
			mForceNumIndices = n;
			mStreamsDirty = true;
		}
		uint32_t getNumIndices() const
		{
			uint32_t n = 0; 
			if (mIndexStream)
			{
				if (0xffffffff == mForceNumIndices)
				{
					n = mIndexStream->getSize() / getNumFormatBytes(mIndexFormat);
				}
				else
				{
					n = mForceNumIndices;
				}
			}
			return n;
		}

		uint32_t getNumInstances() const 
		{
			return 1;
		}

		template<typename tuple_type>
		void bindVertexStream(GraphicsBufferPtr const & buffer, tuple_type const & vertex_elems, StreamType type = ST_Geometry, uint32_t freq = 1)
		{
			this->bindVertexStream(buffer, Tuple2Vector<tuple_type, std::tuple_size<tuple_type>::value>::_do(vertex_elems), type, freq);
		}

		void bindVertexStream(GraphicsBufferPtr const & buffer, VertexElementsType const & vet, StreamType type = ST_Geometry, uint32_t freq = 1);


		void bindIndexStream(GraphicsBufferPtr const & buffer, ElementFormat format);

		void setStartVertexLocation(uint32_t location)
		{
			mStartVertexLocation = location;
			mStreamsDirty = true;
		}

		uint32_t getStartVertexLocation() const
		{
			return mStartVertexLocation;
		}


		void setStartIndexLocation(uint32_t location)
		{
			mStartIndexLocation = location;
			mStreamsDirty = true;
		}

		uint32_t getStartIndexLocation() const
		{
			return mStartIndexLocation;
		}


		uint32_t getStartInstanceLocation() const
		{
			return 0;
		}

		bool useIndices() const;

		GraphicsBufferPtr const & getInstanceStream() const
		{
			return mInstanceStream.mStream;
		}

		void setInstanceStream(GraphicsBufferPtr buffer)
		{
			mInstanceStream.mStream = buffer;
			mStreamsDirty = true;
		}

	private:
		template <typename tuple_type, int N>
		struct Tuple2Vector
		{
			static VertexElementsType _do(tuple_type const & t)
			{
				VertexElementsType ret = Tuple2Vector<tuple_type, N - 1>::_do(t);
				ret.push_back(std::get<N - 1>(t));
				return ret;
			}
		};
		template<typename tuple_type>
		struct Tuple2Vector<tuple_type, 1>
		{
			static VertexElementsType _do(tuple_type const & t)
			{
				return VertexElementsType(1, std::get<0>(t));
			}
		};
	protected:


		TopologyType mTopologyType{ TT_TriangleList };

		struct StreamUnit
		{
			GraphicsBufferPtr mStream;
			VertexElementsType mFormat;
			uint32_t mVertexSize;
			StreamType mType;
			uint32_t mFreq;
		};

		std::vector<StreamUnit> mVertexStreams;
		StreamUnit mInstanceStream;

		GraphicsBufferPtr mIndexStream;
		ElementFormat mIndexFormat{ EF_Unknown };
		uint32_t mForceNumVertices{ 0xffffffff };
		uint32_t mForceNumIndices{ 0xffffffff };
		uint32_t mForceNumInstances{ 0xffffffff };
		mutable bool mStreamsDirty{ true };

		int32_t mBaseVertexLocation{ 0 };

		uint32_t mStartVertexLocation{ 0 };
		uint32_t mStartIndexLocation{ 0 };
		uint32_t mStartInstanceLocation{ 0 };
	};
}




#endif