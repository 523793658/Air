#include "Engine.h"
#include "rendersystem/include/RenderFactory.h"
#include "D3D11RenderEngine.hpp"
#include "D3D11Mapping.hpp"
#include "D3D11Texture.hpp"
namespace Air
{
	D3D11Texture1D::D3D11Texture1D(uint32_t width, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
		:D3D11Texture(Texture::TT_1D, sample_count, sample_quality, access_hint)
	{
		if (0 == num_mip_maps)
		{
			num_mip_maps = 1;
			uint32_t w = width;
			while (w != 1)
			{
				++num_mip_maps;
				w = w >> 1;
			}
		}
		mNumMipMaps = num_mip_maps;
		mArraySize = array_size;
		mFormat = format;
		mDXGIFormat = D3D11Mapping::MappingFormat(mFormat);
		mWidth = width;
	}

	uint32_t D3D11Texture1D::getWidth(uint32_t level) const
	{
		BOOST_ASSERT(level < mNumMipMaps);
		return mWidth >> level;
	}

	void D3D11Texture1D::createHWResource(ArrayRef<ElementInitData> init_data)
	{
		D3D11_TEXTURE1D_DESC desc;
		desc.Width = mWidth;
		desc.ArraySize = mArraySize;
		desc.Format = mDXGIFormat;
		desc.MipLevels = mNumMipMaps;
		this->getD3DFlags(desc.Usage, desc.BindFlags, desc.CPUAccessFlags, desc.MiscFlags);
		std::vector<D3D11_SUBRESOURCE_DATA> sub_data;
		if (!init_data.empty())
		{
			BOOST_ASSERT(init_data.size() == mNumMipMaps);
			sub_data.resize(init_data.size());
			for (size_t i = 0; i < init_data.size(); ++i)
			{
				sub_data[i].pSysMem = init_data[i].data;
				sub_data[i].SysMemPitch = init_data[i].mRowPitch;
				sub_data[i].SysMemSlicePitch = init_data[i].mSlicePitch;
			}
		}
		ID3D11Texture1D* d3d_tex;
		TIFHR(mD3DDevice->CreateTexture1D(&desc, sub_data.data(), &d3d_tex));
		mD3DTexture = MakeComPtr(d3d_tex);
		if ((mAccessHint & (EAH_Generate_Mips | EAH_GPU_Read)) && (mNumMipMaps > 1))
		{
			this->retriveD3DShaderResourceView(0, mArraySize, 0, mNumMipMaps);
		}
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC D3D11Texture1D::fillSRVDesc(uint32_t first_array_index, uint32_t num_items,
		uint32_t first_level, uint32_t num_levels) const
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		switch (mFormat)
		{
		case EF_D16:
			desc.Format = DXGI_FORMAT_R16_UNORM;
			break;
		case EF_D24S8:
			desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			break;
		case EF_D32F:
			desc.Format = DXGI_FORMAT_D32_FLOAT;
			break;
			
		default:
			desc.Format = mDXGIFormat;
			break;
		}
		if (mArraySize > 1)
		{
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
			desc.Texture1DArray.ArraySize = num_items;
			desc.Texture1DArray.FirstArraySlice = first_array_index;
			desc.Texture1DArray.MipLevels = num_levels;
			desc.Texture1DArray.MostDetailedMip = first_level;
		}
		else
		{
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
			desc.Texture1D.MipLevels = num_levels;
			desc.Texture1D.MostDetailedMip = first_level;
		}
		return desc;
	}

	D3D11_RENDER_TARGET_VIEW_DESC D3D11Texture1D::fillRTVDesc(uint32_t first_array_index, uint32_t num_items, uint32_t first_level) const
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		desc.Format = mDXGIFormat;
		if (mArraySize > 1)
		{
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
			desc.Texture1DArray.ArraySize = num_items;
			desc.Texture1DArray.FirstArraySlice = first_array_index;
			desc.Texture1DArray.MipSlice = first_level;
		}
		else
		{
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
			desc.Texture1D.MipSlice = first_level;
		}
		return desc;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC D3D11Texture1D::fillDSVDesc(uint32_t first_array_index, uint32_t array_size, uint32_t level) const
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		desc.Format = mDXGIFormat;
		desc.Flags = 0;
		if (mArraySize > 1)
		{
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
			desc.Texture1DArray.ArraySize = array_size;
			desc.Texture1DArray.FirstArraySlice = first_array_index;
			desc.Texture1DArray.MipSlice = level;

		}
		else
		{
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
			desc.Texture1D.MipSlice = level;
		}
		return desc;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC  D3D11Texture1D::fillUAVDesc(uint32_t first_array_index, uint32_t num_items, uint32_t level) const
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
		desc.Format = mDXGIFormat;
		if (mArraySize > 1)
		{
			desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
			desc.Texture1DArray.ArraySize = num_items;
			desc.Texture1DArray.FirstArraySlice = first_array_index;
			desc.Texture1DArray.MipSlice = level;
		}
		else
		{
			desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
			desc.Texture1D.MipSlice = level;
		}
		return desc;
	}
}