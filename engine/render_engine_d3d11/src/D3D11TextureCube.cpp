#include "Engine.h"
#include "D3D11Mapping.hpp"
#include "D3D11Texture.hpp"

namespace Air
{
	D3D11TextureCube::D3D11TextureCube(uint32_t size, uint32_t numMipMaps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint): D3D11Texture(TT_Cube, sample_count, sample_quality, access_hint)
	{
		if (0 == numMipMaps)
		{
			numMipMaps = 1;
			uint32_t w = size;
			while (w != 1)
			{
				++numMipMaps;
				w = std::max<uint32_t>(1U, w / 2);
			}
		}
		mNumMipMaps = numMipMaps;
		mArraySize = array_size;
		mFormat = format;
		mDXGIFormat = D3D11Mapping::MappingFormat(mFormat);
		mWidth = size;
	}

	uint32_t D3D11TextureCube::getWidth(uint32_t level) const
	{
		BOOST_ASSERT(level < mNumMipMaps);
		return std::max<uint32_t>(1U, mWidth >> level);
	}

	uint32_t D3D11TextureCube::getHeight(uint32_t level) const
	{
		return this->getWidth(level);
	}

	void D3D11TextureCube::createHWResource(ArrayRef<ElementInitData> init_data)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = mWidth;
		desc.Height = mWidth;
		desc.MipLevels = mNumMipMaps;
		desc.ArraySize = 6 * mArraySize;
		desc.Format = D3D11Mapping::MappingFormat(mFormat);
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		this->getD3DFlags(desc.Usage, desc.BindFlags, desc.CPUAccessFlags, desc.MiscFlags);
		desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;

		std::vector<D3D11_SUBRESOURCE_DATA> subres_data;
		if (!init_data.empty())
		{
			BOOST_ASSERT(init_data.size() == 6 * mArraySize * mNumMipMaps);
			subres_data.resize(init_data.size());
			for (size_t i = 0; i < init_data.size(); ++i)
			{
				subres_data[i].pSysMem = init_data[i].data;
				subres_data[i].SysMemPitch = init_data[i].mRowPitch;
				subres_data[i].SysMemSlicePitch = init_data[i].mSlicePitch;
			}
		}
		ID3D11Texture2D* d3d_tex;

		TIFHR(mD3DDevice->CreateTexture2D(&desc, subres_data.data(), &d3d_tex));

		mD3DTexture = MakeComPtr(d3d_tex);
		if ((mAccessHint & (EAH_GPU_Read | EAH_Generate_Mips)) && (mNumMipMaps > 1))
		{
			this->retriveD3DShaderResourceView(0, mArraySize, 0, mNumMipMaps);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC D3D11TextureCube::fillSRVDesc(uint32_t first_array_index, uint32_t num_items,
		uint32_t first_level, uint32_t num_levels) const
	{
		BOOST_ASSERT(0 == first_array_index);
		BOOST_ASSERT(1 == num_items);
		AIR_UNUSED(first_array_index);
		AIR_UNUSED(num_items);
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
			desc.Format = DXGI_FORMAT_R32_FLOAT;
			break;
		default:
			desc.Format = mDXGIFormat;
			break;
		}
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		desc.TextureCube.MipLevels = num_levels;
		desc.TextureCube.MostDetailedMip = first_level;
		return desc;
	}
}