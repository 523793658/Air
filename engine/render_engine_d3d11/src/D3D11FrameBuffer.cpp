#ifndef FLAMEMATH
#define FLAMEMATH
#include "Context.h"
#include "SingletonManager.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "D3D11RenderEngine.hpp"
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
				return checked_cast<D3D11RenderTargetRenderView*>(mColorViews[n].get())->getD3DRenderTargetView();
			}
		}
		return nullptr;
	}
	ID3D11DepthStencilView* D3D11FrameBuffer::getD3DDSView() const
	{
		if (mDepthStencilView)
		{
			return checked_cast<D3D11DepthStencilRenderView*>(mDepthStencilView.get())->getD3DDepthStencilView();
		}
		return nullptr;
	}
	ID3D11UnorderedAccessView* D3D11FrameBuffer::getD3DUAView(uint32_t n) const
	{
		return nullptr;
	}

	std::wstring const & D3D11FrameBuffer::getDescription() const
	{
		static std::wstring const desc(L"Direct3D11 Framebuffer");
		return desc;
	}

	void D3D11FrameBuffer::onBind()
	{
		D3D11RenderEngine& re = *checked_cast<D3D11RenderEngine*>(&SingletonManager::getRenderFactoryInstance().getRenderEngineInstance());
		std::vector<void*> rt_src;
		std::vector<uint32_t> rt_first_subres;
		std::vector<uint32_t> rt_num_subres;
		std::vector<ID3D11RenderTargetView*> rt_view(mColorViews.size());
		for (uint32_t i = 0; i < mColorViews.size(); ++i)
		{
			if (mColorViews[i])
			{
				D3D11RenderTargetRenderView* p = checked_cast<D3D11RenderTargetRenderView*>(mColorViews[i].get());
				rt_src.push_back(p->getSrcTexture().get());
				rt_first_subres.push_back(p->getRTFirstSubRes());
				rt_num_subres.push_back(p->getRTNumSubRes());
				rt_view[i] = p->getD3DRenderTargetView();
			}
			else
			{
				rt_view[i] = nullptr;
			}
		}
		if (mDepthStencilView)
		{
			D3D11DepthStencilRenderView* p = checked_cast<D3D11DepthStencilRenderView*>(mDepthStencilView.get());
			void* p1 = p->getSrcTexture().get();
			BOOST_ASSERT(p1 != nullptr);
			rt_src.push_back(p1);
			rt_first_subres.push_back(p->getRTFirstSubRes());
			rt_num_subres.push_back(p->getRTNumSubRes());
		}

		for (size_t i = 0; i < rt_src.size(); ++i)
		{
			re.detachSRV(rt_src[i], rt_first_subres[i], rt_num_subres[i]);
		}
		re.setRenderTargets(static_cast<UINT>(rt_view.size()), rt_view.size() > 0 ? &rt_view[0] : nullptr, this->getD3DDSView());
		mD3DViewport.TopLeftX = static_cast<float>(mViewport->mLeft);
		mD3DViewport.TopLeftY = static_cast<float>(mViewport->mTop);
		mD3DViewport.Width = static_cast<float>(mViewport->mWidth);
		mD3DViewport.Height = static_cast<float>(mViewport->mHeight);
		re.setViewports(1, &mD3DViewport);
	}
	void D3D11FrameBuffer::onUnbind()
	{

	}

	void D3D11FrameBuffer::clear(uint32_t flags, Color const& clr, float depth, int32_t stencil)
	{
		if (!mNeedClear)
		{
			return;
		}
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


#endif
