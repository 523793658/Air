#ifndef FLAMEMATH
#define FLAMEMATH
#endif
#include "Engine.h"
#include "D3D11RenderView.hpp"
#include "D3D11FrameBuffer.hpp"
namespace Air
{
	D3D11FrameBuffer::D3D11FrameBuffer()
	{
		mLeft = 0;
		mTop = 0;

		mViewport->mLeft = mLeft;
		mViewport->mTop = mTop;
		mD3DViewport.MinDepth = 0.0f;
		mD3DViewport.MaxDepth = 1.0f;

	}
	D3D11FrameBuffer::~D3D11FrameBuffer()
	{

	}

	ID3D11RenderTargetView* D3D11FrameBuffer::getD3DRTView(uint32_t n) const
	{
		if (n < mColorViews.size())
		{
			if (mColorViews[n])
			{
				
			}
		}
		return NULL;
	}
	ID3D11DepthStencilView* D3D11FrameBuffer::getD3DDSView() const
	{
		return NULL;
	}
	ID3D11UnorderedAccessView* D3D11FrameBuffer::getD3DUAView(uint32_t n) const
	{
		return NULL;
	}

	std::wstring const & D3D11FrameBuffer::getDescription() const
	{
		static std::wstring const desc(L"Direct3D11 Framebuffer");
		return desc;
	}

	void D3D11FrameBuffer::onBind()
	{

	}
	void D3D11FrameBuffer::onUnbind()
	{

	}

	void D3D11FrameBuffer::clear(uint32_t flags, Color const& clr, float depth, int32_t stencil)
	{
		if (flags & CBM_Color)
		{
			for (uint32_t i = 0; i < mColorViews.size(); ++i)
			{
				if (mColorViews[i]) 
				{
					mColorViews[i]->clearColor(clr);
				}
			}
		}
		if ((flags & CBM_Depth) && (flags & CBM_Stencil))
		{
			if (mDepthStencilView)
			{
				mDepthStencilView->clearDepthStencil(depth, stencil);
			}
		}
		else
		{
			if (flags & CBM_Depth)
			{
				if (mDepthStencilView)
				{
					mDepthStencilView->clearDepth(depth);
				}
			}
			if (flags & CBM_Stencil)
			{
				if (mDepthStencilView)
				{
					mDepthStencilView->clearStencil(stencil);
				}
			}
		}
	}
	void D3D11FrameBuffer::discard(uint32_t flags)
	{

	}
}
