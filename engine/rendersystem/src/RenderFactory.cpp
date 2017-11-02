#include "Engine.h"
#include <boost/function.hpp>
#include <boost/functional/hash/hash.hpp>
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderStateObject.hpp"

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
	TexturePtr RenderFactory::MakeTexture1D(uint32_t width, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hit, ArrayRef<ElementInitData> init_data)
	{
		TexturePtr ret = this->makeDelayCreationTexture1D(width, num_mip_maps, array_size, format, sample_count, sample_quality, access_hit);
		ret->createHWResource(init_data);
		return ret;
	}

	TexturePtr RenderFactory::MakeTexture2D(uint32_t width, uint32_t height, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint, ArrayRef<ElementInitData> init_data)
	{
		TexturePtr ret = this->makeDelayCreationTexture2D(width, height, num_mip_maps, array_size, format, sample_count, sample_quality, access_hint);
		ret->createHWResource(init_data);
		return ret;
	}

	GraphicsBufferPtr RenderFactory::makeVertexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, void const * init_data, ElementFormat fmt)
	{
		GraphicsBufferPtr ret = this->makeDelayCreationVertexBuffer(usage, access_hint, size_in_byte, fmt);
		ret->createHWResouce(init_data);
		return ret;
	}
	GraphicsBufferPtr RenderFactory::makeIndexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, void const * init_data, ElementFormat fmt)
	{
		GraphicsBufferPtr ret = this->makeDelayCreationIndexBuffer(usage, access_hint, size_in_byte, fmt);
		ret->createHWResouce(init_data);
		return ret;
	}
	GraphicsBufferPtr RenderFactory::makeConstantBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, void const * init_data, ElementFormat fmt)
	{
		GraphicsBufferPtr ret = this->makeDelayCreationConstantBuffer(usage, access_hint, size_in_byte, fmt);
		ret->createHWResouce(init_data);
		return ret;
	}


	RenderStateObjectPtr RenderFactory::makeRenderStateObject(RasterizerStateDesc const & rs_desc, DepthStencilStateDesc const & dss_desc, BlendStateDesc const & bs_desc)
	{
		RenderStateObjectPtr ret;
		char const * rs_desc_begin = reinterpret_cast<char const *>(&rs_desc);
		char const * rs_desc_end = rs_desc_begin + sizeof(rs_desc);

		char const * dds_desc_begin = reinterpret_cast<char const*>(&dss_desc);
		char const * dds_desc_end = dds_desc_begin + sizeof(dss_desc);

		char const * bs_desc_begin = reinterpret_cast<char const*>(&bs_desc);
		char const *bs_desc_end = bs_desc_begin + sizeof(bs_desc);

		size_t seed = boost::hash_range(rs_desc_begin, rs_desc_end);
		boost::hash_range(seed, dds_desc_begin, dds_desc_end);
		boost::hash_range(seed, bs_desc_begin, bs_desc_end);
		auto iter = mRenderStateObjectPool.find(seed);
		if (iter == mRenderStateObjectPool.end())
		{
			ret = this->doMakeRenderStateObject(rs_desc, dss_desc, bs_desc);
			mRenderStateObjectPool.emplace(seed, ret);
		}
		else
		{
			ret = iter->second;
		}
		return ret;

	}


	SamplerStateObjectPtr RenderFactory::MakeSamplerStateObject(SamplerStateDesc const & desc)
	{
		SamplerStateObjectPtr ret;
		char const * desc_begin = reinterpret_cast<char const *>(&desc);
		char const * desc_end = desc_begin + sizeof(desc);
		size_t seed = boost::hash_range(desc_begin, desc_end);
		auto iter = mSamplerStateObjectPool.find(seed);
		if (iter == mSamplerStateObjectPool.end())
		{
			ret = this->doMakeSamplerStateObject(desc);
			mSamplerStateObjectPool.emplace(seed, ret);
		}
		else
		{
			ret = iter->second;
		}
		return ret;
	}
}