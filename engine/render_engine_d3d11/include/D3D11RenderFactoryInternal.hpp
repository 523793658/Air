#ifndef _D3D11RenderFactory_Internal_H_
#define _D3D11RenderFactory_Internal_H_
#pragma once

#include "core/include/PreDeclare.h"
#include "rendersystem/include/RenderFactory.h"

namespace Air
{
	class D3D11RenderFactory : public RenderFactory
	{
	public:
		D3D11RenderFactory();
		std::wstring const & getName() const;

		virtual TexturePtr makeDelayCreationTexture1D(uint32_t width, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint);

		virtual TexturePtr makeDelayCreationTexture2D(uint32_t width, uint32_t height, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint);

		virtual TexturePtr makeDelayCreationTexture3D(uint32_t width, uint32_t height, uint32_t depth, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint);

		virtual TexturePtr makeDelayCreationTextureCube(uint32_t size, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint);

		RenderLayoutPtr MakeRenderLayout();

		virtual GraphicsBufferPtr makeDelayCreationVertexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, ElementFormat fmt = EF_Unknown) override;
		virtual GraphicsBufferPtr makeDelayCreationIndexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, ElementFormat fmt = EF_Unknown) override;
		virtual GraphicsBufferPtr makeDelayCreationConstantBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, ElementFormat fmt = EF_Unknown) override;

		RenderViewPtr Make1DRenderView(Texture& texture, int first_array_index, int array_size, int level);
		RenderViewPtr Make2DRenderView(Texture& texture, int first_array_index, int array_size, int level);
		RenderViewPtr Make2DRenderView(Texture& texture, int array_index, Texture::CubeFaces face, int level);
		RenderViewPtr Make2DRenderView(Texture& texture, int array_index, uint32_t slice, int level);

		RenderViewPtr Make2DDepthStencilRenderView(Texture& texture, int first_array_index, int array_size, int level);

		virtual ShaderObjectPtr makeShaderObject();

		virtual FrameBufferPtr makeFrameBuffer();
	private:
		virtual std::unique_ptr<RenderEngine> doMakeRenderEngine() override;

		virtual RenderStateObjectPtr doMakeRenderStateObject(RasterizerStateDesc const & rs_desc, DepthStencilStateDesc const & dss_desc, BlendStateDesc const & bs_desc) override;

		virtual SamplerStateObjectPtr doMakeSamplerStateObject(SamplerStateDesc const & desc) override;
	};
}


#endif