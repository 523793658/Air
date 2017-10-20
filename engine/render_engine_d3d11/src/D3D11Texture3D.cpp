#include "Engine.h"
#include "D3D11Mapping.hpp"
#include "D3D11Texture.hpp"
namespace Air
{
	D3D11Texture3D::D3D11Texture3D(uint32_t width, uint32_t height, uint32_t depth, uint32_t numMipMaps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
		:D3D11Texture(Texture::TT_3D, sample_count, sample_quality, access_hint)
	{
		BOOST_ASSERT(1 == array_size);
		if (numMipMaps == 0)
		{
			numMipMaps = 1;
			uint32_t w = width;
			uint32_t h = height;
			uint32_t d = depth;
			while ((w != 1) || (h != 1) || (d != 1))
			{
				++numMipMaps;
				w = std::max<uint32_t>(1U, w / 2);
				h = std::max<uint32_t>(1U, h / 2);
				d = std::max<uint32_t>(1U, d / 2);
			}
		}
		mNumMipMaps = numMipMaps;
		mArraySize = array_size;
		mFormat = format;
		mDXGIFormat = D3D11Mapping::MappingFormat(mFormat);
		mWidth = width;
		mHeight = height;
		mDepth = depth;
	}

	uint32_t D3D11Texture3D::getWidth(uint32_t level) const
	{
		BOOST_ASSERT(level < mNumMipMaps);
		return std::max<uint32_t>(1U, mWidth >> level);
	}

	uint32_t D3D11Texture3D::getHeight(uint32_t level) const
	{
		BOOST_ASSERT(level < mNumMipMaps);
		return std::max<uint32_t>(1U, mHeight >> level);
	}

	uint32_t D3D11Texture3D::getDepth(uint32_t level) const
	{
		BOOST_ASSERT(level < mNumMipMaps);
		return std::max<uint32_t>(1U, mDepth >> level);
	}

	void D3D11Texture3D::createHWResource(ArrayRef<ElementInitData> init_data)
	{
		D3D11_TEXTURE3D_DESC desc;
		desc.Width = mWidth;
		desc.Height = mHeight;
		desc.Depth = mDepth;
		desc.MipLevels = mNumMipMaps;
		desc.Format = mDXGIFormat;
		this->getD3DFlags(desc.Usage, desc.BindFlags, desc.CPUAccessFlags, desc.MiscFlags);
		std::vector<D3D11_SUBRESOURCE_DATA> subres_data;
		if (!init_data.empty())
		{
			BOOST_ASSERT(init_data.size() == mNumMipMaps);
			subres_data.resize(init_data.size());
			for (size_t i = 0; i < init_data.size(); ++i)
			{
				subres_data[i].pSysMem = init_data[i].data;
				subres_data[i].SysMemPitch = init_data[i].mRowPitch;
				subres_data[i].SysMemSlicePitch = init_data[i].mSlicePitch;
			}
		}
		ID3D11Texture3D* d3d_tex;
		TIFHR(mD3DDevice->CreateTexture3D(&desc, subres_data.data(), &d3d_tex));
		mD3DTexture = MakeComPtr(d3d_tex);
		if ((mAccessHint & (EAH_GPU_Read | EAH_Generate_Mips)) && (mNumMipMaps > 1))
		{
			this->retriveD3DShaderResourceView(0, mArraySize, 0, mNumMipMaps);
		}
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC D3D11Texture3D::fillSRVDesc(uint32_t first_array_index, uint32_t num_items,
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
			desc.Format = DXGI_FORMAT_R32_FLOAT;
			break;
		default:
			desc.Format = mDXGIFormat;
			break;
		}
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		desc.Texture3D.MipLevels = num_levels;
		desc.Texture3D.MostDetailedMip = first_level;
		return desc;
	}

}