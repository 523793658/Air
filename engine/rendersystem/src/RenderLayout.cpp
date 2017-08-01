#include "Engine.h"

#include "rendersystem/include/RenderLayout.hpp"

namespace Air
{
	void RenderLayout::bindVertexStream(GraphicsBufferPtr const & buffer, VertexElementsType const & vet, StreamType type, uint32_t freq)
	{
		uint32_t size = 0;
		for (size_t i = 0; i < vet.size(); ++i)
		{
			size += vet[i].getElementSize();
		}
		if (ST_Geometry == type)
		{
			for (size_t i = 0; i < mVertexStreams.size(); ++i)
			{
				if (mVertexStreams[i].mFormat == vet)
				{
					mVertexStreams[i].mStream = buffer;
					mVertexStreams[i].mVertexSize = size;
					mVertexStreams[i].mType = type;
					mVertexStreams[i].mFreq = freq;
					return;
				}
			}
			StreamUnit vs;
			vs.mStream = buffer;
			vs.mFormat = vet;
			vs.mVertexSize = size;
			vs.mType = type;
			vs.mFreq = freq;
			mVertexStreams.push_back(vs);
		}
		else
		{
			mInstanceStream.mStream = buffer;
			mInstanceStream.mFormat = vet;
			mInstanceStream.mVertexSize = size;
			mInstanceStream.mType = type;
			mInstanceStream.mFreq = freq;
		}
		mStreamsDirty = true;
	}


	void RenderLayout::bindIndexStream(GraphicsBufferPtr const & buffer, ElementFormat format)
	{
		BOOST_ASSERT((EF_R16UI == format) || (EF_R32UI == format));
		mIndexStream = buffer;
		mIndexFormat = format;
		mStreamsDirty = true;
	}








}