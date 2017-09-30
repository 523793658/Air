#ifndef _D3D11RenderStateObject_H_
#define _D3D11RenderStateObject_H_
#pragma once
#include "PreDeclare.h"
#include "rendersystem/include/RenderStateObject.hpp"
#include "D3D11Typedefs.hpp"
namespace Air
{



	class D3D11RenderStateObject : public RenderStateObject
	{
	public:
		D3D11RenderStateObject(RasterizerStateDesc const & rs_desc, DepthStencilStateDesc const & dss_desc, BlendStateDesc const & bs_desc);

		virtual void active() override;

		ID3D11RasterizerState* getD3DRasterizerState() const
		{
			return mRasterizerState.get();
		}

		ID3D11DepthStencilState* getD3DDepthStencileState() const
		{
			return mDepthStencilState.get();
		}

		ID3D11BlendState* getD3DBlendState() const
		{
			return mBlendState.get();
		}

	private:
		ID3D11RasterizerStatePtr mRasterizerState;
		ID3D11DepthStencilStatePtr mDepthStencilState;
		ID3D11BlendStatePtr mBlendState;
	};


	class D3D11SamplerStateObject : public SamplerStateObject
	{
	public:
		explicit D3D11SamplerStateObject(SamplerStateDesc const & desc);

		ID3D11SamplerState* getD3DSamplerState() const
		{
			return mSamplerState.get();
		}
	private:
		ID3D11SamplerStatePtr mSamplerState;
	};

}



#endif
