#include "Engine.h"
#include "Context.h"
#include "basic/include/ErrorHanding.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "D3D11Mapping.hpp"
#include "D3D11RenderEngine.hpp"
#include "D3D11GraphicsBuffer.hpp"
namespace Air
{
	D3D11GraphicsBuffer::D3D11GraphicsBuffer(BufferUsage usage, uint32_t access_hint, uint32_t bind_flags, uint32_t size_in_byte, ElementFormat fmt)
		:GraphicsBuffer(usage, access_hint, size_in_byte), mBindFlags(bind_flags), mFmtAsShaderRes(fmt)
	{
		if ((access_hint & EAH_GPU_Structured) && (mFmtAsShaderRes != EF_Unknown))
		{
			mBindFlags = 0;
		}
		D3D11RenderEngine const & renderEngine(*checked_cast<D3D11RenderEngine const *>(&Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance()));
		mD3DDevice = renderEngine.getD3DDevice();
		mD3DImmCtx = renderEngine.getD3DDeviceContext();
	}

	ID3D11RenderTargetViewPtr const & D3D11GraphicsBuffer::getD3DRenderTargetView() const
	{
		if (mBuffer && !mD3DRTView)
		{
			D3D11_RENDER_TARGET_VIEW_DESC desc;
			desc.Format = D3D11Mapping::MappingFormat(mFmtAsShaderRes);
			desc.ViewDimension = D3D11_RTV_DIMENSION_BUFFER;
			desc.Buffer.ElementOffset = 0;
			desc.Buffer.ElementWidth = this->getSize() / getNumFormatBytes(mFmtAsShaderRes);

			ID3D11RenderTargetView* rt_view;
			TIFHR(mD3DDevice->CreateRenderTargetView(mBuffer.get(), &desc, &rt_view));
			mD3DRTView = MakeComPtr(rt_view);
		}
		return mD3DRTView;
	}

	void D3D11GraphicsBuffer::getD3DFlags(D3D11_USAGE& usage, UINT& cpu_access_flags, UINT& bind_flags, UINT& misc_flags)
	{
		if (mAccessHint & EAH_Immutable)
		{
			usage = D3D11_USAGE_IMMUTABLE;
		}
		else
		{
			if ((EAH_CPU_Write == mAccessHint) || ((EAH_CPU_Write | EAH_GPU_Read) == mAccessHint))
			{
				usage = D3D11_USAGE_DYNAMIC;
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
		cpu_access_flags = 0;
		if (mAccessHint & EAH_CPU_Read)
		{
			cpu_access_flags |= D3D11_CPU_ACCESS_READ;
		}
		if (mAccessHint & EAH_CPU_Write)
		{
			cpu_access_flags |= D3D11_CPU_ACCESS_WRITE;
		}
		if (D3D11_USAGE_STAGING == usage)
		{
			bind_flags = 0;
		}
		else
		{
			bind_flags = mBindFlags;
		}
		D3D11RenderEngine const & re = *checked_cast<D3D11RenderEngine const *>(&Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance());
		if(re.getDeviceFeatureLevel() > D3D_FEATURE_LEVEL_9_3)
		{
			if ((mAccessHint & EAH_GPU_Read) && !(mAccessHint & EAH_CPU_Write))
			{
				bind_flags |= D3D11_BIND_SHADER_RESOURCE;
			}
			if (mAccessHint & EAH_GPU_Write)
			{
				if (!((mAccessHint & EAH_GPU_Structured) || (mAccessHint & EAH_GPU_Unordered)))
				{
					bind_flags |= D3D11_BIND_STREAM_OUTPUT;
				}
			}
			if (mAccessHint & EAH_GPU_Unordered)
			{
				bind_flags |= D3D11_BIND_UNORDERED_ACCESS;
			}
		}
		misc_flags = 0;
		if (mAccessHint & EAH_GPU_Unordered)
		{
			misc_flags |= (mAccessHint & EAH_GPU_Structured) ? D3D11_RESOURCE_MISC_BUFFER_STRUCTURED : D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
		}
		if (mAccessHint & EAH_DrawIndirectArgs)
		{
			misc_flags |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
		}
	}

	void D3D11GraphicsBuffer::createHWResouce(void const * init_data)
	{
		D3D11_SUBRESOURCE_DATA subRes_init;
		D3D11_SUBRESOURCE_DATA* p_subres = nullptr;
		if (init_data != nullptr)
		{
			subRes_init.pSysMem = init_data;
			subRes_init.SysMemPitch = mSizeInByte;
			subRes_init.SysMemSlicePitch = mSizeInByte;
			p_subres = &subRes_init;
		}
		D3D11_BUFFER_DESC desc = {};
		this->getD3DFlags(desc.Usage, desc.CPUAccessFlags, desc.BindFlags, desc.MiscFlags);
		desc.ByteWidth = mSizeInByte;
		desc.StructureByteStride = getNumFormatBytes(mFmtAsShaderRes);
		ID3D11Buffer* buffer;
		TIFHR(mD3DDevice->CreateBuffer(&desc, p_subres, &buffer));
		mBuffer = MakeComPtr(buffer);
		if ((mAccessHint & EAH_GPU_Read) && (mFmtAsShaderRes != EF_Unknown))
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC sr_desc;
			sr_desc.Format = (mAccessHint & EAH_GPU_Structured) ? DXGI_FORMAT_UNKNOWN : D3D11Mapping::MappingFormat(mFmtAsShaderRes);
			sr_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			sr_desc.Buffer.ElementOffset = 0;
			sr_desc.Buffer.ElementWidth = mSizeInByte / desc.StructureByteStride;
			ID3D11ShaderResourceView* d3d_sr_view;
			TIFHR(mD3DDevice->CreateShaderResourceView(mBuffer.get(), &sr_desc, &d3d_sr_view));
			mD3DShaderResourceView = MakeComPtr(d3d_sr_view);
		}
		if ((mAccessHint & EAH_GPU_Unordered) && (mFmtAsShaderRes != EF_Unknown))
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
			if (mAccessHint & EAH_Raw)
			{
				uav_desc.Format = DXGI_FORMAT_R32_TYPELESS;
			}
			else if (mAccessHint & EAH_GPU_Structured)
			{
				uav_desc.Format = DXGI_FORMAT_UNKNOWN;
			}
			else
			{
				uav_desc.Format = D3D11Mapping::MappingFormat(mFmtAsShaderRes);
			}
			uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uav_desc.Buffer.FirstElement = 0;
			uav_desc.Buffer.NumElements = mSizeInByte / desc.StructureByteStride;
			uav_desc.Buffer.Flags = 0;
			if (mAccessHint & EAH_Raw)
			{
				uav_desc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_RAW;
			}
			if (mAccessHint & EAH_Append)
			{
				uav_desc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_APPEND;
			}
			if (mAccessHint & EAH_Counter)
			{
				uav_desc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_COUNTER;
			}
			ID3D11UnorderedAccessView* d3d_ua_view;
			TIFHR(mD3DDevice->CreateUnorderedAccessView(mBuffer.get(), &uav_desc, &d3d_ua_view));
			mD3DUAView = MakeComPtr(d3d_ua_view);
		}
	}
	void D3D11GraphicsBuffer::deleteHWResource()
	{
		mD3DShaderResourceView.reset();
		mD3DUAView.reset();
		mBuffer.reset();
	}

	void D3D11GraphicsBuffer::updateSubResource(uint32_t offset, uint32_t size, void const * data)
	{
		D3D11_BOX* p = nullptr;
		D3D11_BOX box;
		if (!(mBindFlags & D3D11_BIND_CONSTANT_BUFFER))
		{
			p = &box;
			box.left = offset;
			box.top = 0;
			box.front = 0;
			box.right = offset + size;
			box.bottom = 1;
			box.back = 1;
		}
		mD3DImmCtx->UpdateSubresource(mBuffer.get(), 0, p, data, size, size);
	}
	void * D3D11GraphicsBuffer::map(BufferAccess ba)
	{
		BOOST_ASSERT(mBuffer);
		D3D11_MAP type;
		switch (ba)
		{
		case Air::BA_Read_Only:
			type = D3D11_MAP_READ;
			break;
		case Air::BA_Write_Only:
			if ((EAH_CPU_Write == mAccessHint) || (EAH_CPU_Write | EAH_GPU_Read) == mAccessHint)
			{
				type = D3D11_MAP_WRITE_DISCARD;
			}
			else
			{
				D3D11_MAP_WRITE;
			}
			break;
		case Air::BA_Read_Write:
			type = D3D11_MAP_READ_WRITE;
			break;
		case Air::BA_Write_No_Overwrite:
			type = D3D11_MAP_WRITE_NO_OVERWRITE;
			break;
		default:
			BOOST_ASSERT(false);
			type = D3D11_MAP_READ;
			break;
		}
		D3D11_MAPPED_SUBRESOURCE mapped;
		TIFHR(mD3DImmCtx->Map(mBuffer.get(), 0, type, 0, &mapped));
		return mapped.pData;
	}
	void D3D11GraphicsBuffer::unmap()
	{
		BOOST_ASSERT(mBuffer);
		mD3DImmCtx->Unmap(mBuffer.get(), 0);
	}

	void D3D11GraphicsBuffer::copyToBuffer(GraphicsBuffer& rhs)
	{
		BOOST_ASSERT(this->getSize() <= rhs.getSize());
		D3D11GraphicsBuffer& d3d_gb = *checked_cast<D3D11GraphicsBuffer*>(&rhs);
		if (this->getSize() == rhs.getSize())
		{
			mD3DImmCtx->CopyResource(d3d_gb.getD3DBuffer(), mBuffer.get());
		}
		else
		{
			D3D11_BOX box;
			box.left = 0;
			box.right = this->getSize();
			box.front = 0;
			box.top = 0;
			box.bottom = 1;
			box.back = 1;
			mD3DImmCtx->CopySubresourceRegion(d3d_gb.getD3DBuffer(), 0, 0, 0, 0, mBuffer.get(), 0, &box);
		}
	}

}