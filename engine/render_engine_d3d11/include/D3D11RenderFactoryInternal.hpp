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

		virtual TexturePtr MakeDelayCreationTexture2D(uint32_t width, uint32_t height, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint);

		RenderLayoutPtr MakeRenderLayout();

		RenderViewPtr Make1DRenderView(Texture& texture, int first_array_index, int array_size, int level);
		RenderViewPtr Make2DRenderView(Texture& texture, int first_array_index, int array_size, int level);
		RenderViewPtr Make2DRenderView(Texture& texture, int array_index, Texture::CubeFace face, int level);
		RenderViewPtr Make2DRenderView(Texture& texture, int array_index, uint32_t slice, int level);

		RenderViewPtr Make2DDepthStencilRenderView(Texture& texture, int first_array_index, int array_size, int level);
	private:
		virtual std::unique_ptr<RenderEngine> doMakeRenderEngine() override;
	};
}


#endif