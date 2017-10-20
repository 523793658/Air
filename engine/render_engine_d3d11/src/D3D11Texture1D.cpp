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
}