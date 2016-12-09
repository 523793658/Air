#ifndef _RenderFactory_H_
#define _RenderFactory_H_
#pragma once

#include "PreDeclare.h"
#include "RenderEngine.hpp"
#include "rendersystem/include/Texture.hpp"

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

		virtual RenderViewPtr Make1DRenderView(Texture& texture, int first_array_index, int array_size, int level) = 0;
		virtual RenderViewPtr Make2DRenderView(Texture& texture, int first_array_index, int array_size, int level) = 0;
		virtual RenderViewPtr Make2DRenderView(Texture& texture, int array_index, Texture::CubeFace face, int level) = 0;
		virtual RenderViewPtr Make2DRenderView(Texture& texture, int array_index, uint32_t slice, int level) = 0;

		virtual RenderViewPtr Make2DDepthStencilRenderView(Texture& texture, int first_array_index, int array_size, int level) = 0;

		TexturePtr MakeTexture2D(uint32_t width, uint32_t height, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint, ElementInitData const * init_data);
	private:
		virtual std::unique_ptr<RenderEngine> doMakeRenderEngine() = 0;

	private:
		std::unique_ptr<RenderEngine> mRenderEnginePtr;
	};
}




#endif