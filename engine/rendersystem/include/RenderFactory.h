#ifndef _RenderFactory_H_
#define _RenderFactory_H_
#pragma once
#include <unordered_map>
#include "PreDeclare.h"
#include "rendersystem/include/Texture.hpp"
#include "rendersystem/include/GraphicsBuffer.hpp"
#include "rendersystem/include/RenderEngine.hpp"

namespace Air
{
	class AIR_CORE_API RenderFactory
	{
	public:
		virtual ~RenderFactory();

		virtual std::wstring const & getName() const = 0;

		RenderEngine& getRenderEngineInstance();

		virtual TexturePtr MakeDelayCreationTexture2D(uint32_t width, uint32_t height, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint) = 0;

		virtual RenderLayoutPtr MakeRenderLayout() = 0;

		virtual GraphicsBufferPtr makeDelayCreationVertexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, ElementFormat fmt = EF_Unknown) = 0;
		virtual GraphicsBufferPtr makeDelayCreationIndexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte,  ElementFormat fmt = EF_Unknown) = 0;
		virtual GraphicsBufferPtr makeDelayCreationConstantBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, ElementFormat fmt = EF_Unknown) = 0;

		GraphicsBufferPtr makeVertexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, void const * init_data, ElementFormat fmt = EF_Unknown);
		GraphicsBufferPtr makeIndexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, void const * init_data, ElementFormat fmt = EF_Unknown);
		GraphicsBufferPtr makeConstantBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, void const * init_data, ElementFormat fmt = EF_Unknown);

		virtual RenderViewPtr Make1DRenderView(Texture& texture, int first_array_index, int array_size, int level) = 0;
		virtual RenderViewPtr Make2DRenderView(Texture& texture, int first_array_index, int array_size, int level) = 0;
		virtual RenderViewPtr Make2DRenderView(Texture& texture, int array_index, Texture::CubeFaces face, int level) = 0;
		virtual RenderViewPtr Make2DRenderView(Texture& texture, int array_index, uint32_t slice, int level) = 0;

		virtual RenderViewPtr Make2DDepthStencilRenderView(Texture& texture, int first_array_index, int array_size, int level) = 0;

		TexturePtr MakeTexture2D(uint32_t width, uint32_t height, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint, ArrayRef<ElementInitData> init_data = {});

		SamplerStateObjectPtr MakeSamplerStateObject(SamplerStateDesc const & desc);

		RenderStateObjectPtr RenderFactory::makeRenderStateObject(RasterizerStateDesc const & rs_desc, DepthStencilStateDesc const & dss_desc, BlendStateDesc const & bs_desc);

		virtual ShaderObjectPtr makeShaderObject() = 0;
	private:
		virtual std::unique_ptr<RenderEngine> doMakeRenderEngine() = 0;
		virtual RenderStateObjectPtr doMakeRenderStateObject(RasterizerStateDesc const & rs_desc, DepthStencilStateDesc const & dss_desc, BlendStateDesc const & bs_desc) = 0;
		virtual SamplerStateObjectPtr doMakeSamplerStateObject(SamplerStateDesc const & desc) = 0;
	protected:
		std::unique_ptr<RenderEngine> mRenderEnginePtr;
		std::unordered_map<size_t, RenderStateObjectPtr> mRenderStateObjectPool;
		std::unordered_map<size_t, SamplerStateObjectPtr> mSamplerStateObjectPool;
	};
}




#endif