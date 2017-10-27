#include "Engine.h"
#include "basic/include/Util.h"
#include "D3D11RenderEngine.hpp"
#include "D3D11RenderLayout.hpp"
#include "D3D11RenderView.hpp"
#include "D3D11Texture.hpp"
#include "D3D11ShaderObject.hpp"
#include "D3D11RenderStateObject.hpp"
#include "D3D11RenderFactory.hpp"
#include "D3D11FrameBuffer.hpp"
#include "D3D11GraphicsBuffer.hpp"
#include "D3D11RenderFactoryInternal.hpp"

namespace Air
{
	D3D11RenderFactory::D3D11RenderFactory()
	{

	}
	TexturePtr D3D11RenderFactory::makeDelayCreationTexture1D(uint32_t width, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
	{
		return MakeSharedPtr<D3D11Texture1D>(width, num_mip_maps, array_size, format, sample_count, sample_quality, access_hint);
	}


	TexturePtr D3D11RenderFactory::makeDelayCreationTexture2D(uint32_t width, uint32_t height, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
	{
		return MakeSharedPtr<D3D11Texture2D>(width, height, num_mip_maps, array_size, format, sample_count, sample_quality, access_hint);
	}

	TexturePtr D3D11RenderFactory::makeDelayCreationTexture3D(uint32_t width, uint32_t height, uint32_t depth, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
	{
		return MakeSharedPtr<D3D11Texture3D>(width, height, depth, num_mip_maps, array_size, format, sample_count, sample_quality, access_hint);
	}

	TexturePtr D3D11RenderFactory::makeDelayCreationTextureCube(uint32_t size, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
	{
		return MakeSharedPtr<D3D11TextureCube>(size, num_mip_maps, array_size, format, sample_count, sample_quality, access_hint);
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

	RenderViewPtr D3D11RenderFactory::Make1DRenderView(TexturePtr const & texture, int first_array_index, int array_size, int level)
	{
		return nullptr;
	}
	RenderViewPtr D3D11RenderFactory::Make2DRenderView(TexturePtr const & texture, int first_array_index, int array_size, int level)
	{
		return MakeSharedPtr<D3D11RenderTargetRenderView>(texture, first_array_index, array_size, level);
	}
	RenderViewPtr D3D11RenderFactory::Make2DRenderView(TexturePtr const & texture, int array_index, Texture::CubeFaces face, int level)
	{
		//return MakeSharedPtr<D3D11RenderTargetRenderView>()
		return nullptr;
	}
	RenderViewPtr D3D11RenderFactory::Make2DRenderView(TexturePtr const & texture, int array_index, uint32_t slice, int level)
	{
		return nullptr;
	}
	RenderViewPtr D3D11RenderFactory::Make2DDepthStencilRenderView(TexturePtr const & texture, int first_array_index, int array_size, int level)
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

	ShaderObjectPtr D3D11RenderFactory::makeShaderObject()
	{
		return MakeSharedPtr<D3D11ShaderObject>();
	}

	FrameBufferPtr D3D11RenderFactory::makeFrameBuffer()
	{
		return MakeSharedPtr<D3D11FrameBuffer>();
	}

	RenderStateObjectPtr D3D11RenderFactory::doMakeRenderStateObject(RasterizerStateDesc const & rs_desc, DepthStencilStateDesc const & dss_desc, BlendStateDesc const & bs_desc)
	{
		return MakeSharedPtr<D3D11RenderStateObject>(rs_desc, dss_desc, bs_desc);
	}

	SamplerStateObjectPtr D3D11RenderFactory::doMakeSamplerStateObject(SamplerStateDesc const & desc)
	{
		return MakeSharedPtr<D3D11SamplerStateObject>(desc);
	}


}

void makeRenderFactory(std::unique_ptr<Air::RenderFactory>& ptr)
{
	ptr = Air::MakeUniquePtr<Air::D3D11RenderFactory>();
}
