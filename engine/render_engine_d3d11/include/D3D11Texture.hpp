#ifndef _D3D11Texture_H_
#define _D3D11Texture_H_
#pragma once
#include <unordered_map>
#include "rendersystem/include/Texture.hpp"
#include "D3D11Typedefs.hpp"
#include "D3D11RenderView.hpp"
namespace Air
{
	class D3D11Texture : public Texture
	{
	public:
		D3D11Texture(TextureType type, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint);
		virtual ~D3D11Texture();


		//子类去实现，由于1d，2d，3d纹理的长、宽、深度定义不同
		uint32_t getWidth(uint32_t level) const;
		uint32_t getHeight(uint32_t level) const;
		uint32_t getDepth(uint32_t level) const;


		virtual ID3D11RenderTargetViewPtr const & retriveD3DRenderTargetView(uint32_t first_array_index, uint32_t array_size, uint32_t level);

		virtual ID3D11DepthStencilViewPtr const & retriveD3DDepthStencilView(uint32_t first_array_index, uint32_t array_size, uint32_t level);

		virtual ID3D11ShaderResourceViewPtr const & retriveD3DShaderResourceView(uint32_t first_array_index, uint32_t num_items, uint32_t first_level, uint32_t num_levels);

		virtual ID3D11UnorderedAccessViewPtr const & retriveD3DUnorderedAccessView(uint32_t first_array_index, uint32_t num_items, uint32_t level);

		virtual void deleteHWResource() override;
		virtual bool isHWResourceReady() const override;

		ID3D11Resource* getD3D11Resource() const
		{
			return mD3DTexture.get();
		}


	protected:
		ID3D11RenderTargetViewPtr const & retriveD3DRTV(D3D11_RENDER_TARGET_VIEW_DESC const & desc);
		ID3D11DepthStencilViewPtr const & retriveD3DDSV(D3D11_DEPTH_STENCIL_VIEW_DESC const & desc);
		ID3D11ShaderResourceViewPtr const & retriveD3DSRV(D3D11_SHADER_RESOURCE_VIEW_DESC const & desc);
		void getD3DFlags(D3D11_USAGE& usage, UINT& bind_flags, UINT& cpu_access_flags, UINT& misc_flags);

	private:
		virtual D3D11_UNORDERED_ACCESS_VIEW_DESC fillUAVDesc(uint32_t first_array_index, uint32_t num_items, uint32_t level) const;
		virtual D3D11_UNORDERED_ACCESS_VIEW_DESC fillUAVDesc(uint32_t array_index, uint32_t first_slice, uint32_t num_slices,
			uint32_t level) const;
		virtual D3D11_UNORDERED_ACCESS_VIEW_DESC fillUAVDesc(uint32_t first_array_index, uint32_t num_items,
			CubeFaces first_face, uint32_t num_faces, uint32_t level) const;

		virtual D3D11_SHADER_RESOURCE_VIEW_DESC fillSRVDesc(uint32_t first_array_index, uint32_t num_items,
			uint32_t first_level, uint32_t num_levels) const;

		virtual D3D11_RENDER_TARGET_VIEW_DESC fillRTVDesc(uint32_t first_array_index, uint32_t num_items, uint32_t first_level) const;

		virtual D3D11_DEPTH_STENCIL_VIEW_DESC fillDSVDesc(uint32_t first_array_index, uint32_t array_size, uint32_t level) const;
	protected:
		ID3D11Device* mD3DDevice;
		ID3D11DeviceContext* mD3DImmContext;
		DXGI_FORMAT mDXGIFormat;
		ID3D11ResourcePtr mD3DTexture;
		std::unordered_map<size_t, ID3D11ShaderResourceViewPtr> mD3DShaderResourceViews;
		std::unordered_map<size_t, ID3D11UnorderedAccessViewPtr> mD3DUnorderedAccessViews;
		std::unordered_map<size_t, ID3D11RenderTargetViewPtr> mD3DRenderTargetViews;
		std::unordered_map<size_t, ID3D11DepthStencilViewPtr> mD3DDepthStencilViews;
	};

	class D3D11Texture1D : public D3D11Texture
	{
	public:
		D3D11Texture1D(uint32_t width, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint);

		virtual void createHWResource(ArrayRef<ElementInitData> init_data) override;
		uint32_t getWidth(uint32_t level) const;
	private:
		uint32_t mWidth;
	};

	class D3D11Texture2D : public D3D11Texture
	{
	public:
		D3D11Texture2D(uint32_t width, uint32_t height, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint);

		explicit D3D11Texture2D(ID3D11Texture2DPtr const & d3d_tex);

		virtual void createHWResource(ArrayRef<ElementInitData> init_data) override;

		uint32_t getWidth(uint32_t level) const;
		uint32_t getHeight(uint32_t level) const;

	private:
		virtual D3D11_SHADER_RESOURCE_VIEW_DESC D3D11Texture2D::fillSRVDesc(uint32_t first_array_index, uint32_t num_items,
			uint32_t first_level, uint32_t num_levels) const override;

		D3D11_RENDER_TARGET_VIEW_DESC fillRTVDesc(uint32_t first_array_index, uint32_t num_items, uint32_t first_level) const override;

		D3D11_DEPTH_STENCIL_VIEW_DESC fillDSVDesc(uint32_t first_array_index, uint32_t array_size, uint32_t level) const override;

		virtual D3D11_UNORDERED_ACCESS_VIEW_DESC fillUAVDesc(uint32_t first_array_index, uint32_t num_items, uint32_t level) const override;
	private:
		uint32_t mWidth;
		uint32_t mHeight;
	};

	class D3D11Texture3D : public D3D11Texture
	{
	public:
		D3D11Texture3D(uint32_t width, uint32_t height, uint32_t depth, uint32_t numMipMaps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint);

		uint32_t getWidth(uint32_t level) const;
		uint32_t getHeight(uint32_t level) const;
		uint32_t getDepth(uint32_t level) const;



		virtual void createHWResource(ArrayRef<ElementInitData> init_data) override;
	private:
		virtual D3D11_SHADER_RESOURCE_VIEW_DESC fillSRVDesc(uint32_t first_array_index, uint32_t num_items,
			uint32_t first_level, uint32_t num_levels) const override;
	private:
		uint32_t mWidth;
		uint32_t mHeight;
		uint32_t mDepth;
	};

	class D3D11TextureCube : public D3D11Texture
	{
	public:
		D3D11TextureCube(uint32_t size, uint32_t numMipMaps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint);

		uint32_t getWidth(uint32_t level) const;
		uint32_t getHeight(uint32_t level) const;
		virtual void createHWResource(ArrayRef<ElementInitData> init_data) override;
	private:
		virtual D3D11_SHADER_RESOURCE_VIEW_DESC fillSRVDesc(uint32_t first_array_index, uint32_t num_items,
			uint32_t first_level, uint32_t num_levels) const override;
	private:

		uint32_t mWidth;
	};
}



#endif