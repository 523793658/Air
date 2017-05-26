#include "Engine.h"
#include "rendersystem/include/RenderFactory.h"


namespace Air
{
	RenderFactory::~RenderFactory()
	{
	}

	RenderEngine& RenderFactory::getRenderEngineInstance()
	{
		if (!mRenderEnginePtr)
		{
			mRenderEnginePtr = this->doMakeRenderEngine();
		}
		return *mRenderEnginePtr;
	}

	TexturePtr RenderFactory::MakeTexture2D(uint32_t width, uint32_t height, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint, ElementInitData const * init_data)
	{
		TexturePtr ret = this->MakeDelayCreationTexture2D(width, height, num_mip_maps, array_size, format, sample_count, sample_quality, access_hint);
		ret->createHWResource(init_data);
		return ret;
	}

	GraphicsBufferPtr RenderFactory::makeVertexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, void const * init_data, ElementFormat fmt)
	{
		GraphicsBufferPtr ret = this->makeDelayCreationVertexBuffer(usage, access_hint, size_in_byte, init_data, fmt);
		ret->createHWResouce(init_data);
		return ret;
	}
	GraphicsBufferPtr RenderFactory::makeIndexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, void const * init_data, ElementFormat fmt)
	{
		GraphicsBufferPtr ret = this->makeDelayCreationIndexBuffer(usage, access_hint, size_in_byte, init_data, fmt);
		ret->createHWResouce(init_data);
		return ret;
	}
	GraphicsBufferPtr RenderFactory::makeConstantBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, void const * init_data, ElementFormat fmt)
	{
		GraphicsBufferPtr ret = this->makeDelayCreationConstantBuffer(usage, access_hint, size_in_byte, init_data, fmt);
		ret->createHWResouce(init_data);
		return ret;
	}
}