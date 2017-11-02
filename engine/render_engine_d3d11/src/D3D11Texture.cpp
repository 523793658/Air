#include "Engine.h"
#include "boost/functional/hash.hpp"
#include "ElementFormat.h"
#include "Context.h"
#include "D3D11RenderFactory.hpp"
#include "D3D11RenderEngine.hpp"
#include "D3D11Texture.hpp"

namespace Air
{

	D3D11Texture::D3D11Texture(TextureType type, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
		: Texture(type, sample_count, sample_quality, access_hint)
	{
		if (access_hint & EAH_GPU_Write)
		{
			BOOST_ASSERT(!(access_hint & EAH_CPU_Read));
			BOOST_ASSERT(!(access_hint & EAH_CPU_Write));
		}
		D3D11RenderEngine& renderEngine(*checked_cast<D3D11RenderEngine*>(&Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance()));
		mD3DDevice = renderEngine.getD3DDevice();
		mD3DImmContext = renderEngine.getD3DDeviceContext();
	}

	D3D11Texture::~D3D11Texture()
	{

	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC D3D11Texture::fillUAVDesc(uint32_t first_array_index, uint32_t num_items, uint32_t level) const
	{
		AIR_UNUSED(first_array_index);
		AIR_UNUSED(num_items);
		AIR_UNUSED(level);
		AIR_UNREACHABLE("invalid invoke");
	}
	D3D11_UNORDERED_ACCESS_VIEW_DESC D3D11Texture::fillUAVDesc(uint32_t array_index, uint32_t first_slice, uint32_t num_slices,
		uint32_t level) const
	{
		AIR_UNUSED(array_index);
		AIR_UNUSED(first_slice);
		AIR_UNUSED(num_slices);
		AIR_UNUSED(num_slices);
		AIR_UNREACHABLE("invalid invoke");
	}
	D3D11_UNORDERED_ACCESS_VIEW_DESC D3D11Texture::fillUAVDesc(uint32_t first_array_index, uint32_t num_items,
		CubeFaces first_face, uint32_t num_faces, uint32_t level) const
	{
		AIR_UNUSED(first_array_index);
		AIR_UNUSED(num_items);
		AIR_UNUSED(first_face);
		AIR_UNUSED(num_faces);
		AIR_UNUSED(level);
		AIR_UNREACHABLE("invalid invoke");
	}

	D3D11_RENDER_TARGET_VIEW_DESC D3D11Texture::fillRTVDesc(uint32_t first_array_index, uint32_t num_items, uint32_t first_level) const
	{
		AIR_UNUSED(first_level);
		AIR_UNUSED(first_array_index);
		AIR_UNUSED(num_items);
		AIR_UNREACHABLE("Can't be called");
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC D3D11Texture::fillDSVDesc(uint32_t first_array_index, uint32_t array_size, uint32_t level) const
	{
		AIR_UNUSED(first_array_index);
		AIR_UNUSED(array_size);
		AIR_UNUSED(level);
		AIR_UNREACHABLE("Can't be called");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC D3D11Texture::fillSRVDesc(uint32_t first_array_index, uint32_t num_items,
		uint32_t first_level, uint32_t num_levels) const
	{
		AIR_UNUSED(first_array_index);
		AIR_UNUSED(num_items);
		AIR_UNUSED(first_level);
		AIR_UNUSED(num_levels);
		AIR_UNREACHABLE("invalid invoke");
	}

	void D3D11Texture::deleteHWResource()
	{
		mD3DShaderResourceViews.clear();
		mD3DUnorderedAccessViews.clear();
		mD3DDepthStencilViews.clear();
		mD3DRenderTargetViews.clear();
		mD3DTexture.reset();
	}

	bool D3D11Texture::isHWResourceReady() const
	{
		return mD3DTexture.get() ? true : false;
	}

	ID3D11RenderTargetViewPtr const & D3D11Texture::retriveD3DRTV(D3D11_RENDER_TARGET_VIEW_DESC const & desc)
	{
		if (this->isHWResourceReady())
		{
			char const *p = reinterpret_cast<char const *>(&desc);
			size_t hash_val = boost::hash_range(p, p + sizeof(desc));
			auto iter = mD3DRenderTargetViews.find(hash_val);
			if (iter != mD3DRenderTargetViews.end())
			{
				return iter->second;
			}
			ID3D11RenderTargetView* rt_view;
			mD3DDevice->CreateRenderTargetView(this->getD3D11Resource(), &desc, &rt_view);
			return mD3DRenderTargetViews.emplace(hash_val, MakeComPtr(rt_view)).first->second;
		}
		else
		{
			static ID3D11RenderTargetViewPtr view;
			return view;
		}
	}

	ID3D11DepthStencilViewPtr const & D3D11Texture::retriveD3DDSV(D3D11_DEPTH_STENCIL_VIEW_DESC const & desc)
	{
		if (this->isHWResourceReady())
		{
			char const* p = reinterpret_cast<char const *>(&desc);
			size_t hash_val = 0;
			boost::hash_range(hash_val, p, p + sizeof(desc));
			auto iter = mD3DDepthStencilViews.find(hash_val);
			if (iter != mD3DDepthStencilViews.end())
			{
				return iter->second;
			}

			ID3D11DepthStencilView* ds_view;
			mD3DDevice->CreateDepthStencilView(this->getD3D11Resource(), &desc, &ds_view);
			return mD3DDepthStencilViews.emplace(hash_val, MakeComPtr(ds_view)).first->second;
		}
		else
		{
			static ID3D11DepthStencilViewPtr view;
			return view;
		}
	}
	ID3D11ShaderResourceViewPtr const & D3D11Texture::retriveD3DSRV(D3D11_SHADER_RESOURCE_VIEW_DESC const & desc)
	{
		if (this->isHWResourceReady())
		{
			char const *p = reinterpret_cast<char const *>(&desc);
			size_t hash_val = 0;
			boost::hash_range(hash_val, p, p + sizeof(desc));
			auto iter = mD3DShaderResourceViews.find(hash_val);
			if (iter != mD3DShaderResourceViews.end())
			{
				return iter->second;
			}
			ID3D11ShaderResourceView* d3d_sr_view;
			mD3DDevice->CreateShaderResourceView(this->getD3D11Resource(), &desc, &d3d_sr_view);
			return mD3DShaderResourceViews.emplace(hash_val, MakeComPtr(d3d_sr_view)).first->second;
		}
		else
		{
			static ID3D11ShaderResourceViewPtr view;
			return view;
		}
	}

	ID3D11UnorderedAccessViewPtr const & D3D11Texture::retriveD3DUnorderedAccessView(uint32_t first_array_index, uint32_t num_items, uint32_t level)
	{
		BOOST_ASSERT(this->getAccesshint() & EAH_GPU_Unordered);
		if (this->isHWResourceReady())
		{
			size_t hash_val = boost::hash_value(first_array_index);
			boost::hash_combine(hash_val, num_items);
			boost::hash_combine(hash_val, level);
			boost::hash_combine(hash_val, 0);
			boost::hash_combine(hash_val, 0);
			auto iter = mD3DUnorderedAccessViews.find(hash_val);
			if (iter != mD3DUnorderedAccessViews.end())
			{
				return iter->second;
			}
			else
			{
				auto desc = this->fillUAVDesc(first_array_index, num_items, level);
				ID3D11UnorderedAccessView* d3d_ua_view;
				mD3DDevice->CreateUnorderedAccessView(this->getD3D11Resource(), &desc, &d3d_ua_view);
				return mD3DUnorderedAccessViews.emplace(hash_val, MakeComPtr(d3d_ua_view)).first->second;
			}
		}
		else
		{
			static ID3D11UnorderedAccessViewPtr const view;
			return view;
		}
	}

	uint32_t D3D11Texture::getWidth(uint32_t level) const
	{
		return 1;
	}
	uint32_t D3D11Texture::getHeight(uint32_t level) const
	{
		BOOST_ASSERT(level < mNumMipMaps);
		return 1;
	}
	uint32_t D3D11Texture::getDepth(uint32_t level) const
	{
		BOOST_ASSERT(level < mNumMipMaps);
		return 1;
	}


	void D3D11Texture::getD3DFlags(D3D11_USAGE& usage, UINT& bind_flags, UINT& cpu_access_flags, UINT& misc_flags)
	{
		if (mAccessHint & EAH_Immutable)
		{
			usage = D3D11_USAGE_IMMUTABLE;
		}
		else
		{
			if ((EAH_CPU_Write | EAH_GPU_Read) == mAccessHint)
			{
				usage = D3D11_USAGE_DYNAMIC;
			}
			else
			{
				if (EAH_CPU_Write == mAccessHint)
				{
					if ((mNumMipMaps != 1) || (TT_Cube == mType))
					{
						usage = D3D11_USAGE_STAGING;
					}
					else
					{
						usage = D3D11_USAGE_DYNAMIC;
					}
				}
				else
				{
					if (!(mAccessHint & EAH_CPU_Read) && !(mAccessHint & EAH_CPU_Write))
					{
						usage = D3D11_USAGE_DEFAULT;
					}
					else
					{
						usage = D3D11_USAGE_STAGING;
					}
				}
			}
		}
		bind_flags = 0;
		if ((mAccessHint & EAH_GPU_Read) || (D3D11_USAGE_DYNAMIC == usage))
		{
			bind_flags |= D3D11_BIND_SHADER_RESOURCE;
		}
		if (mAccessHint & EAH_GPU_Write)
		{
			if (isDepthFormat(mFormat))
			{
				bind_flags |= D3D11_BIND_DEPTH_STENCIL;
			}
			else
			{
				bind_flags |= D3D11_BIND_RENDER_TARGET;
			}
		}
		D3D11RenderEngine const & engine = *checked_cast<D3D11RenderEngine const*>(&Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance());

		if (engine.getDeviceFeatureLevel() >= D3D_FEATURE_LEVEL_11_0)
		{
			if (mAccessHint & EAH_GPU_Unordered)
			{
				bind_flags |= D3D11_BIND_UNORDERED_ACCESS;
			}
		}
		cpu_access_flags = 0;
		if (mAccessHint & EAH_CPU_Read)
		{
			cpu_access_flags |= D3D11_CPU_ACCESS_READ;
		}
		if (mAccessHint & EAH_CPU_Write)
		{
			cpu_access_flags |= D3D11_CPU_ACCESS_WRITE;
		}
		misc_flags = 0;
		if (mAccessHint & EAH_Generate_Mips)
		{
			misc_flags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}
	}

	ID3D11RenderTargetViewPtr const & D3D11Texture::retriveD3DRenderTargetView(uint32_t first_array_index, uint32_t array_size, uint32_t level)
	{
		BOOST_ASSERT(this->getAccesshint() & EAH_GPU_Write);
		BOOST_ASSERT(first_array_index < this->getArraySize());
		BOOST_ASSERT(first_array_index + array_size <= this->getArraySize());
		if (this->isHWResourceReady())
		{
			size_t hash_val = boost::hash_value(first_array_index);
			boost::hash_combine(hash_val, array_size);
			boost::hash_combine(hash_val, level);
			boost::hash_combine(hash_val, 0);
			boost::hash_combine(hash_val, 0);
			auto iter = mD3DRenderTargetViews.find(hash_val);
			if (iter != mD3DRenderTargetViews.end())
			{
				return iter->second;
			}
			else
			{
				auto desc = this->fillRTVDesc(first_array_index, array_size, level);
				ID3D11RenderTargetView* rt_view;
				mD3DDevice->CreateRenderTargetView(mD3DTexture.get(), &desc, &rt_view);
				return mD3DRenderTargetViews.emplace(hash_val, MakeComPtr(rt_view)).first->second;
			}
		}
		else
		{
			static ID3D11RenderTargetViewPtr const view;
			return view;
		}
	}

	ID3D11DepthStencilViewPtr const & D3D11Texture::retriveD3DDepthStencilView(uint32_t first_array_index, uint32_t array_size, uint32_t level)
	{
		BOOST_ASSERT(this->mAccessHint & EAH_GPU_Write);
		BOOST_ASSERT(first_array_index < this->mArraySize);
		BOOST_ASSERT(first_array_index + array_size <= this->mArraySize);
		if (this->isHWResourceReady())
		{
			size_t hash_val = boost::hash_value(first_array_index);
			boost::hash_combine(hash_val, array_size);
			boost::hash_combine(hash_val, level);
			boost::hash_combine(hash_val, 0);
			boost::hash_combine(hash_val, 0);
			auto iter = mD3DDepthStencilViews.find(hash_val);
			if (iter != mD3DDepthStencilViews.end())
			{
				return iter->second;
			}
			else
			{
				auto desc = this->fillDSVDesc(first_array_index, array_size, level);
				ID3D11DepthStencilView* d3d_view;
				TIFHR(mD3DDevice->CreateDepthStencilView(mD3DTexture.get(), &desc, &d3d_view));
				return mD3DDepthStencilViews.emplace(hash_val, MakeComPtr(d3d_view)).first->second;
			}
		}
		else
		{
			static ID3D11DepthStencilViewPtr const view;
			return view;
		}
	}

	ID3D11ShaderResourceViewPtr const & D3D11Texture::retriveD3DShaderResourceView(uint32_t first_array_index, uint32_t num_items, uint32_t first_level, uint32_t num_levels)
	{
		BOOST_ASSERT(this->mAccessHint & EAH_GPU_Read);
		if (this->isHWResourceReady())
		{
			size_t hash_val = boost::hash_value(first_array_index);
			boost::hash_combine(hash_val, num_items);
			boost::hash_combine(hash_val, first_level);
			boost::hash_combine(hash_val, num_levels);
			auto iter = mD3DShaderResourceViews.find(hash_val);
			if (iter != mD3DShaderResourceViews.end())
			{
				return iter->second;
			}
			else
			{
				auto desc = this->fillSRVDesc(first_array_index, num_items, first_level, num_levels);
				ID3D11ShaderResourceView* d3d_sr_view;
				mD3DDevice->CreateShaderResourceView(mD3DTexture.get(), &desc, &d3d_sr_view);
				return mD3DShaderResourceViews.emplace(hash_val, MakeComPtr(d3d_sr_view)).first->second;
			}
		}
		else
		{
			static ID3D11ShaderResourceViewPtr const view;
			return view;
		}
	}
}