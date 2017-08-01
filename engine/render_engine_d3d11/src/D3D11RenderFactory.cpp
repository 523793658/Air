#include "Engine.h"
#include "basic/include/Util.h"
#include "D3D11RenderEngine.hpp"
#include "D3D11RenderLayout.hpp"
#include "D3D11RenderView.hpp"
#include "D3D11Texture.hpp"
#include "render_engine_d3d11/include/D3D11RenderFactory.hpp"
#include "render_engine_d3d11/include/D3D11RenderFactoryInternal.hpp"
#include "D3D11GraphicsBuffer.hpp"
namespace Air
{
	D3D11RenderFactory::D3D11RenderFactory()
	{

	}

	TexturePtr D3D11RenderFactory::MakeDelayCreationTexture2D(uint32_t width, uint32_t height, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
	{
		return MakeSharedPtr<D3D11Texture2D>(width, height, num_mip_maps, array_size, format, sample_count, sample_quality, access_hint);
	}


	RenderLayoutPtr D3D11RenderFactory::MakeRenderLayout()
	{
		return MakeSharedPtr<D3D11RenderLayout>();
	}

	std::unique_ptr<RenderEngine> D3D11RenderFactory::doMakeRenderEngine()
	{
		return MakeUniquePtr<D3D11RenderEngine>();
	}
	std::wstring const & D3D11RenderFactory::getName() const
	{
		static std::wstring const name(L"Direct3D11 Render Factory");
		return name;
	}

	RenderViewPtr D3D11RenderFactory::Make1DRenderView(Texture& texture, int first_array_index, int array_size, int level)
	{
		return nullptr;
	}
	RenderViewPtr D3D11RenderFactory::Make2DRenderView(Texture& texture, int first_array_index, int array_size, int level)
	{
		return MakeSharedPtr<D3D11RenderTargetRenderView>(texture, first_array_index, array_size, level);
	}
	RenderViewPtr D3D11RenderFactory::Make2DRenderView(Texture& texture, int array_index, Texture::CubeFace face, int level)
	{
		//return MakeSharedPtr<D3D11RenderTargetRenderView>()
		return nullptr;
	}
	RenderViewPtr D3D11RenderFactory::Make2DRenderView(Texture& texture, int array_index, uint32_t slice, int level)
	{
		return nullptr;
	}
	RenderViewPtr D3D11RenderFactory::Make2DDepthStencilRenderView(Texture& texture, int first_array_index, int array_size, int level)
	{
		return MakeSharedPtr<D3D11DepthStencilRenderView>(texture, first_array_index, array_size, level);
	}

	GraphicsBufferPtr D3D11RenderFactory::makeDelayCreationVertexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, ElementFormat fmt)
	{
		return MakeSharedPtr<D3D11GraphicsBuffer>(usage, access_hint, D3D11_BIND_VERTEX_BUFFER, size_in_byte, fmt);
	}
	GraphicsBufferPtr D3D11RenderFactory::makeDelayCreationIndexBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, ElementFormat fmt)
	{
		return MakeSharedPtr<D3D11GraphicsBuffer>(usage, access_hint, D3D11_BIND_INDEX_BUFFER, size_in_byte, fmt);
	}
	GraphicsBufferPtr D3D11RenderFactory::makeDelayCreationConstantBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte, ElementFormat fmt)
	{
		return MakeSharedPtr<D3D11GraphicsBuffer>(usage, access_hint, D3D11_BIND_CONSTANT_BUFFER, size_in_byte, fmt);
	}
}

void makeRenderFactory(std::unique_ptr<Air::RenderFactory>& ptr)
{
	ptr = Air::MakeUniquePtr<Air::D3D11RenderFactory>();
}
