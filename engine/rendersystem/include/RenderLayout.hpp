#ifndef _Air_Render_Layout_H_
#define _Air_Render_Layout_H_
#pragma once
#include "PreDeclare.h"

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
			TT_32_Ctrl_Pt_PatchList
		};

		enum StreamType
		{
			ST_Geometry,
			ST_Instance
		};

		void setTopologyType(TopologyType type)
		{
			mTopologyType = type;
		}

		GraphicsBufferPtr const & getVertexStream(uint32_t index) const
		{
			return mVertexStreams[index].mStream;
		}
		void setVertexStream(uint32_t index, GraphicsBufferPtr const & gb)
		{
			mVertexStreams[index].mStream = gb;
			mStreamsDirty = true;
		}

		uint32_t getNumVertexStreams() const
		{
			return static_cast<uint32_t>(mVertexStreams.size());
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


		TopologyType mTopologyType;

		struct StreamUnit
		{
			GraphicsBufferPtr mStream;
			VertexElementsType mFormat;
			uint32_t mVertexSize;
			StreamType mType;
			uint32_t mFreq;
		};

		std::vector<StreamUnit> mVertexStreams;
		mutable bool mStreamsDirty;
	};
}




#endif