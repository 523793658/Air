#include "Engine.h"
#include <boost/assert.hpp>
#include "ElementFormat.h"
#include "basic/include/ThrowErr.hpp"
#include "D3D11RenderEngine.hpp"
#include "D3D11RenderFactory.hpp"
#include "D3D11Mapping.hpp"
#include "D3D11Texture.hpp"
namespace Air
{
	D3D11Texture2D::D3D11Texture2D(uint32_t width, uint32_t height, uint32_t num_mip_maps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
		:D3D11Texture(TT_2D, sample_count, sample_quality, access_hint)
	{
		if (0 == num_mip_maps)
		{
			num_mip_maps = 1;
			uint32_t w = width;
			uint32_t h = height;
			while ((w != 1) || (h != 1))
			{
				++num_mip_maps;
				w = std::max<uint32_t>(1U, w / 2);
				h = std::max<uint32_t>(1U, h / 2);
			}
		}
		mNumMipMaps = num_mip_maps;
		D3D11RenderEngine const & re = *checked_cast<D3D11RenderEngine const *>(&Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance());
		if (re.getDeviceFeatureLevel() <= D3D_FEATURE_LEVEL_9_3)
		{
			if (!re.getDeviceCaps().mFullNoptTextureSupport && (mNumMipMaps > 1) && (((width & (width - 1)) != 0 || ((height & (height - 1)) != 0))))
			{
				num_mip_maps = 1;
			}
			if ((num_mip_maps > 1) && isCompressedFormat(format))
			{
				uint32_t clamped_num_mip_maps;
				for (clamped_num_mip_maps = 0; clamped_num_mip_maps < num_mip_maps; ++clamped_num_mip_maps)
				{
					uint32_t w = std::max<uint32_t>(1U, width >> clamped_num_mip_maps);
					uint32_t h = std::max<uint32_t>(1U, height >> clamped_num_mip_maps);
					if (((w & 0x3) != 0) || ((h & 0x3) != 0))
					{
						break;
					}
				}
				mNumMipMaps = clamped_num_mip_maps;
			}
		}
		mArraySize= array_size;
		mFormat = format;
		mDXGIFormat = D3D11Mapping::MappingFormat(mFormat);
		mWidth = width;
		mHeight = height;
	}

	D3D11Texture2D::D3D11Texture2D(ID3D11Texture2DPtr const & d3d_tex)
		:D3D11Texture(TT_2D, 1, 0, 0)
	{
		D3D11_TEXTURE2D_DESC desc;
		d3d_tex->GetDesc(&desc);
		mNumMipMaps = desc.MipLevels;
		mArraySize = desc.ArraySize;
		mFormat = D3D11Mapping::MappingFormat(desc.Format);
		mSampleCount = desc.SampleDesc.Count;
		mSampleQuality = desc.SampleDesc.Quality;
		mWidth = desc.Width;
		mHeight = desc.Height;
		mAccessHint = 0;
		switch (desc.Usage)
		{
		case D3D11_USAGE_DEFAULT:
			mAccessHint |= EAH_GPU_Read | EAH_GPU_Write;
			break;
		case D3D11_USAGE_IMMUTABLE:
			mAccessHint |= EAH_Immutable;
			break;
		case D3D11_USAGE_DYNAMIC:
			mAccessHint |= EAH_GPU_Read;
			if (desc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE)
			{
				mAccessHint |= EAH_CPU_Write;
			}
			break;
		case D3D11_USAGE_STAGING:
			if (desc.CPUAccessFlags & D3D11_CPU_ACCESS_READ)
			{
				mAccessHint |= EAH_CPU_Read;
			}
			if (desc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE)
			{
				mAccessHint |= EAH_CPU_Write;
			}
			break;
		default:
			BOOST_ASSERT(false);
			break;
		}
		if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			mAccessHint |= EAH_GPU_Unordered;
		}
		if (desc.BindFlags & D3D11_RESOURCE_MISC_GENERATE_MIPS)
		{
			mAccessHint |= EAH_Generate_Mips;
		}
		mD3DTexture = d3d_tex;
		if ((mAccessHint & (EAH_GPU_Read | EAH_Generate_Mips)) && (mNumMipMaps > 1))
		{
			this->retriveD3DShaderResourceView(0, mArraySize, 0, mNumMipMaps);
		}

	}


	ID3D11RenderTargetViewPtr const & D3D11Texture2D::retriveD3DRenderTargetView(uint32_t first_array_index, uint32_t array_size, uint32_t level)
	{
		BOOST_ASSERT(this->getAccesshint() & EAH_GPU_Write);
		BOOST_ASSERT(first_array_index < this->getArraySize());
		BOOST_ASSERT(first_array_index + array_size <= this->getArraySize());

		D3D11_RENDER_TARGET_VIEW_DESC desc;
		desc.Format = D3D11Mapping::MappingFormat(this->getFormat());
		if (this->mSampleCount > 1)
		{
			if (this->mArraySize > 1)
			{
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
			}
			else
			{
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
			}
		}
		else
		{
			if (this->mArraySize > 1)
			{
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			}
			else
			{
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			}
		}
		if (this->mArraySize > 1)
		{
			desc.Texture2DArray.MipSlice = level;
			desc.Texture2DArray.FirstArraySlice = first_array_index;
			desc.Texture2DArray.ArraySize = array_size;
		}
		else
		{
			desc.Texture2D.MipSlice = level;
		}
		return this->retriveD3DRTV(desc);
	}

	uint32_t D3D11Texture2D::getWidth(uint32_t level) const
	{
		BOOST_ASSERT(level < mNumMipMaps);
		return std::max<uint32_t>(1U, mWidth >> level);
	}
	uint32_t D3D11Texture2D::getHeight(uint32_t level) const
	{
		BOOST_ASSERT(level < mNumMipMaps);
		return std::max<uint32_t>(1U, mHeight >> level);
	}
	ID3D11DepthStencilViewPtr const & D3D11Texture2D::retriveD3DDepthStencilView(uint32_t first_array_index, uint32_t array_size, uint32_t level)
	{
		BOOST_ASSERT(this->getAccesshint() & EAH_GPU_Write);
		BOOST_ASSERT(first_array_index < this->getArraySize());
		BOOST_ASSERT(first_array_index + array_size <= this->getArraySize());

		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		desc.Format = D3D11Mapping::MappingFormat(this->getFormat());
		desc.Flags = 0;
		if (this->mSampleCount > 1)
		{
			if (this->getArraySize() > 1)
			{
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
			}
			else
			{
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			}
		}
		else
		{
			if (this->getArraySize() > 1)
			{
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			}
			else
			{
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			}
		}
		if (this->getArraySize() > 1)
		{
			desc.Texture2DArray.MipSlice = level;
			desc.Texture2DArray.FirstArraySlice = first_array_index;
			desc.Texture2DArray.ArraySize = array_size;
		}
		else
		{
			desc.Texture2D.MipSlice = level;
		}
		return this->retriveD3DDSV(desc);
	}

	ID3D11ShaderResourceViewPtr const & D3D11Texture2D::retriveD3DShaderResourceView(uint32_t first_array_index, uint32_t num_items,
		uint32_t first_level, uint32_t num_levels) {
		BOOST_ASSERT(this->mAccessHint & EAH_GPU_Read);
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		switch (mFormat)
		{
		case Air::EF_D16:
			desc.Format = DXGI_FORMAT_R16_UNORM;
			break;
		case Air::EF_D24S8:
			desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			break;
		case Air::EF_D32F:
			desc.Format = DXGI_FORMAT_R32_FLOAT;
			break;
		default:
			desc.Format = mDXGIFormat;
			break;
		}
		if (mArraySize > 1)
		{
			if (mSampleCount > 1)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
			}
			else
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			}
		}
		else
		{
			if (mSampleCount > 1)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
			}
			else
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			}
			desc.Texture2D.MostDetailedMip = first_level;
			desc.Texture2D.MipLevels = num_levels;
		}
		return this->retriveD3DSRV(desc);
	}

	void D3D11Texture2D::createHWResource(ElementInitData const * init_data)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = mWidth;
		desc.Height = mHeight;
		desc.MipLevels = mNumMipMaps;
		desc.ArraySize = mArraySize;
		switch (mFormat)
		{
		case EF_D16:
			desc.Format = DXGI_FORMAT_R16_TYPELESS;
			break;
		case EF_D24S8:
			desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
			break;
		case EF_D32F:
			desc.Format = DXGI_FORMAT_R32_TYPELESS;
		default:
			desc.Format = mDXGIFormat;
			break;
		}
		desc.SampleDesc.Count = mSampleCount;
		desc.SampleDesc.Quality = mSampleQuality;
		this->getD3DFlags(desc.Usage, desc.BindFlags, desc.CPUAccessFlags, desc.MiscFlags);
		std::vector<D3D11_SUBRESOURCE_DATA> subres_data(mArraySize * mNumMipMaps);
		if (init_data != nullptr)
		{
			for (uint32_t j = 0; j < mArraySize; ++j)
			{
				for (uint32_t i = 0; i < mNumMipMaps; ++i)
				{
					subres_data[j * mNumMipMaps + i].pSysMem = init_data[j * mNumMipMaps + i].data;
					subres_data[j * mNumMipMaps + i].SysMemPitch = init_data[j * mNumMipMaps + i].mRowPitch;
					subres_data[j * mNumMipMaps + i].SysMemSlicePitch = init_data[j * mNumMipMaps + i].mSlicePitch;
				}
			}
		}
		ID3D11Texture2D* d3d_tex;
		HRESULT r = mD3DDevice->CreateTexture2D(&desc, (init_data != nullptr) ? &subres_data[0] : nullptr, &d3d_tex);
		TIF(r);
		mD3DTexture = MakeComPtr(d3d_tex);
		//TODO shader×ÊÔ´
		if ((mAccessHint & (EAH_GPU_Read | EAH_Generate_Mips)) && (mNumMipMaps > 1))
		{
			/*this->*/
		}
	}
}