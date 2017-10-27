#include "Engine.h"
#include "rendersystem/include/RenderFactory.h"
#include "D3D11Mapping.hpp"
#include "D3D11RenderEngine.hpp"
#include "D3D11RenderStateObject.hpp"

namespace Air
{
	D3D11RenderStateObject::D3D11RenderStateObject(RasterizerStateDesc const & rs_desc, DepthStencilStateDesc const & dss_desc, BlendStateDesc const & bs_desc): RenderStateObject(rs_desc, dss_desc, bs_desc)
	{
		D3D11RenderEngine& re = *checked_cast<D3D11RenderEngine*>(&Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance());
		ID3D11Device* d3d_device = re.getD3DDevice();
		RenderDeviceCaps const & caps = re.getDeviceCaps();
		D3D11_RASTERIZER_DESC d3d_rs_desc;
		d3d_rs_desc.FillMode = D3D11Mapping::mapping(rs_desc.mPolygonMode);
		d3d_rs_desc.CullMode = D3D11Mapping::mapping(rs_desc.mCullMode);
		d3d_rs_desc.AntialiasedLineEnable = false;
		d3d_rs_desc.DepthBias = static_cast<int>(rs_desc.mPolygonOffsetUnits);
		d3d_rs_desc.DepthBiasClamp = rs_desc.mPolygonOffsetUnits;
		d3d_rs_desc.DepthClipEnable = rs_desc.mDepthClipEnable;
		d3d_rs_desc.FrontCounterClockwise = rs_desc.mFrontFaceCCW;
		d3d_rs_desc.MultisampleEnable = rs_desc.mMultiSampleEnabel;
		d3d_rs_desc.ScissorEnable = rs_desc.mScissorEnable;
		d3d_rs_desc.SlopeScaledDepthBias = rs_desc.mPolygonOffsetFactor;

		if (re.getD3D11RuntimeSubVer() >= 1)
		{
			ID3D11Device1* d3d_device_1 = re.getD3DDevice1();
			D3D11_RASTERIZER_DESC1 d3d_rs_desc1;
			d3d_rs_desc1.AntialiasedLineEnable = d3d_rs_desc.AntialiasedLineEnable;
			d3d_rs_desc1.CullMode = d3d_rs_desc.CullMode;
			d3d_rs_desc1.DepthBias = d3d_rs_desc.DepthBias;
			d3d_rs_desc1.DepthBiasClamp = d3d_rs_desc.DepthBiasClamp;
			d3d_rs_desc1.DepthClipEnable = d3d_rs_desc.DepthClipEnable;
			d3d_rs_desc1.FillMode = d3d_rs_desc.FillMode;
			d3d_rs_desc1.ForcedSampleCount = 0;
			d3d_rs_desc1.FrontCounterClockwise = d3d_rs_desc.FrontCounterClockwise;
			d3d_rs_desc1.MultisampleEnable = d3d_rs_desc.MultisampleEnable;
			d3d_rs_desc1.ScissorEnable = d3d_rs_desc.ScissorEnable;
			d3d_rs_desc1.SlopeScaledDepthBias = d3d_rs_desc.SlopeScaledDepthBias;
			ID3D11RasterizerState1* rasterizer_state;
			TIFHR(d3d_device_1->CreateRasterizerState1(&d3d_rs_desc1, &rasterizer_state));
			mRasterizerState = MakeComPtr(rasterizer_state);
		}
		else
		{
			ID3D11RasterizerState* rasterizer_state;
			TIFHR(d3d_device->CreateRasterizerState(&d3d_rs_desc, &rasterizer_state));
			mRasterizerState = MakeComPtr(rasterizer_state);
		}

		D3D11_DEPTH_STENCIL_DESC d3d_dss_desc;
		d3d_dss_desc.BackFace.StencilDepthFailOp = D3D11Mapping::mapping(dss_desc.mBackStencilDepthFail);
		d3d_dss_desc.BackFace.StencilFailOp = D3D11Mapping::mapping(dss_desc.mBackStencilFail);
		d3d_dss_desc.BackFace.StencilFunc = D3D11Mapping::mapping(dss_desc.mBackStencilFunc);
		d3d_dss_desc.BackFace.StencilPassOp = D3D11Mapping::mapping(dss_desc.mBackStencilPass);
		d3d_dss_desc.DepthEnable = dss_desc.mDepthEnable;
		d3d_dss_desc.DepthFunc = D3D11Mapping::mapping(dss_desc.mDepthFunc);
		d3d_dss_desc.DepthWriteMask = D3D11Mapping::mapping(dss_desc.mDepthWriteEnable);
		d3d_dss_desc.FrontFace.StencilDepthFailOp = D3D11Mapping::mapping(dss_desc.mFrontStencilDepthFail);
		d3d_dss_desc.FrontFace.StencilFailOp = D3D11Mapping::mapping(dss_desc.mFrontStencilFail);
		d3d_dss_desc.FrontFace.StencilFunc = D3D11Mapping::mapping(dss_desc.mFrontStencilFunc);
		d3d_dss_desc.FrontFace.StencilPassOp = D3D11Mapping::mapping(dss_desc.mFrontStencilPass);
		d3d_dss_desc.StencilEnable = dss_desc.mFrontStencilEnable;
		d3d_dss_desc.StencilReadMask = static_cast<uint8_t>(dss_desc.mFrontstenCilReadMask);
		d3d_dss_desc.StencilWriteMask = static_cast<uint8_t>(dss_desc.mFrontStencilWriteMask);

		ID3D11DepthStencilState* ds_state;
		TIFHR(d3d_device->CreateDepthStencilState(&d3d_dss_desc, &ds_state));
		mDepthStencilState = MakeComPtr(ds_state);

		D3D11_BLEND_DESC d3d_bs_desc;
		d3d_bs_desc.AlphaToCoverageEnable = bs_desc.mAlphaToCoverageEnable;
		d3d_bs_desc.IndependentBlendEnable =   caps.mIndependentBlendSupport ?bs_desc.mIndependentBlendEnable : false;
		for (int i = 0; i < 8; ++i)
		{
			uint32_t const rt_index = caps.mIndependentBlendSupport ? i : 0;
			d3d_bs_desc.RenderTarget[i].BlendEnable = bs_desc.mBlendEnable[rt_index];
			d3d_bs_desc.RenderTarget[i].BlendOp = D3D11Mapping::mapping(bs_desc.mBlendOp[rt_index]);
			d3d_bs_desc.RenderTarget[i].BlendOpAlpha = D3D11Mapping::mapping(bs_desc.mBlendOpAlpha[rt_index]);
			d3d_bs_desc.RenderTarget[i].DestBlend = D3D11Mapping::mapping(bs_desc.mDstBlend[rt_index]);
			d3d_bs_desc.RenderTarget[i].DestBlendAlpha = D3D11Mapping::mapping(bs_desc.mDstBlendAlpha[rt_index]);
			d3d_bs_desc.RenderTarget[i].RenderTargetWriteMask = static_cast<UINT8>(D3D11Mapping::MappingColorMask(bs_desc.mColorWriteMask[rt_index]));
			d3d_bs_desc.RenderTarget[i].SrcBlend = D3D11Mapping::mapping(bs_desc.mSrcBlend[rt_index]);
			d3d_bs_desc.RenderTarget[i].SrcBlendAlpha = D3D11Mapping::mapping(bs_desc.mSrcBlendAlpha[rt_index]);
		}
		if (re.getD3D11RuntimeSubVer() >= 1)
		{
			ID3D11Device1* d3d_device_1 = re.getD3DDevice1();
			D3D11_BLEND_DESC1 d3d_bs_desc1;
			d3d_bs_desc1.AlphaToCoverageEnable = d3d_bs_desc.AlphaToCoverageEnable;
			d3d_bs_desc1.IndependentBlendEnable = d3d_bs_desc.IndependentBlendEnable;
			for (int i = 0; i < 8; ++i)
			{
				uint32_t const rt_index = caps.mIndependentBlendSupport ? i : 0;
				d3d_bs_desc1.RenderTarget[i].BlendEnable = d3d_bs_desc.RenderTarget[i].BlendEnable;
				d3d_bs_desc1.RenderTarget[i].BlendOp = d3d_bs_desc.RenderTarget[i].BlendOp;
				d3d_bs_desc1.RenderTarget[i].BlendOpAlpha = d3d_bs_desc.RenderTarget[i].BlendOpAlpha;
				d3d_bs_desc1.RenderTarget[i].DestBlend = d3d_bs_desc.RenderTarget[i].DestBlend;
				d3d_bs_desc1.RenderTarget[i].DestBlendAlpha = d3d_bs_desc.RenderTarget[i].DestBlendAlpha;
				d3d_bs_desc1.RenderTarget[i].RenderTargetWriteMask = d3d_bs_desc.RenderTarget[i].RenderTargetWriteMask;
				d3d_bs_desc1.RenderTarget[i].SrcBlend = d3d_bs_desc.RenderTarget[i].SrcBlend;
				d3d_bs_desc1.RenderTarget[i].SrcBlendAlpha = d3d_bs_desc.RenderTarget[i].SrcBlendAlpha;
				d3d_bs_desc1.RenderTarget[i].LogicOp = D3D11Mapping::mapping(bs_desc.mLogicOp[rt_index]);
				d3d_bs_desc1.RenderTarget[i].LogicOpEnable = bs_desc.mLogicOpEnable[rt_index];
			}
			ID3D11BlendState1* blend_state;
			TIFHR(d3d_device_1->CreateBlendState1(&d3d_bs_desc1, &blend_state));
			mBlendState = MakeComPtr(blend_state);
		}
		else
		{
			ID3D11BlendState* blend_state;
			TIFHR(d3d_device->CreateBlendState(&d3d_bs_desc, &blend_state));
			mBlendState = MakeComPtr(blend_state);
		}
	}
	void D3D11RenderStateObject::active()
	{
		D3D11RenderEngine& re = *checked_cast<D3D11RenderEngine*>(&Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance());
		re.setRasterizerState(mRasterizerState.get());
		re.setDepthStencilState(mDepthStencilState.get(), mDSSDesc.mFrontStencilRef);
		re.setBlendState(mBlendState.get(), mBSDesc.mBlendFactor, mBSDesc.mSampleMask);
	}

	D3D11SamplerStateObject::D3D11SamplerStateObject(SamplerStateDesc const & desc)
		:SamplerStateObject(desc)
	{
		D3D11RenderEngine& re = *checked_cast<D3D11RenderEngine*>(&Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance());
		ID3D11Device* d3d_device = re.getD3DDevice();
		D3D11_SAMPLER_DESC d3d_desc;
		d3d_desc.AddressU = D3D11Mapping::mapping(desc.mAddrModeU);
		d3d_desc.AddressV = D3D11Mapping::mapping(desc.mAddrModeV);
		d3d_desc.AddressW = D3D11Mapping::mapping(desc.mAddrModeW);
		d3d_desc.BorderColor[0] = desc.mBorderColor.r();
		d3d_desc.BorderColor[1] = desc.mBorderColor.g();
		d3d_desc.BorderColor[2] = desc.mBorderColor.b();
		d3d_desc.BorderColor[3] = desc.mBorderColor.a();

		d3d_desc.ComparisonFunc = D3D11Mapping::mapping(desc.mCmpFunc);
		d3d_desc.Filter = D3D11Mapping::mapping(desc.mFilter);
		d3d_desc.MaxAnisotropy = desc.mMaxAnisotropy;
		d3d_desc.MaxLOD = desc.mMaxLod;
		d3d_desc.MinLOD = desc.mMinLod;
		d3d_desc.MipLODBias = desc.mMiPMapLoadBias;
		ID3D11SamplerState* sampler_state;
		TIFHR(d3d_device->CreateSamplerState(&d3d_desc, &sampler_state));
		mSamplerState = MakeComPtr(sampler_state);
	}
}