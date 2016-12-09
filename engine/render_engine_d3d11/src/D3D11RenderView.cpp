#include "Engine.h"
#include "Context.h"
#include "rendersystem/include/RenderFactory.h"
#include "D3D11RenderEngine.hpp"
#include "D3D11Texture.hpp"
#include "D3D11RenderView.hpp"
namespace Air
{
	D3D11RenderView::D3D11RenderView()
	{
		D3D11RenderEngine& renderEngine(*checked_cast<D3D11RenderEngine*>(&Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance()));
		mD3DDevice = renderEngine.getD3DDevice();
		mD3DImmContext = renderEngine.getD3DDeviceContext();
		mD3DImmContext1 = renderEngine.getD3DDeviceContext1();
	}

	D3D11RenderView::D3D11RenderView(Texture* texture)
	{
		D3D11RenderEngine& renderEngine(*checked_cast<D3D11RenderEngine*>(&Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance()));
		mD3DDevice = renderEngine.getD3DDevice();
		mD3DImmContext = renderEngine.getD3DDeviceContext();
		mD3DImmContext1 = renderEngine.getD3DDeviceContext1();
		mSrcTexture = texture;
	}

	D3D11RenderView::~D3D11RenderView()
	{

	}

	void D3D11RenderView::onAttached(FrameBuffer& fb, uint32_t att)
	{

	}
	void D3D11RenderView::onDetached(FrameBuffer& fb, uint32_t att)
	{

	}

	D3D11RenderTargetRenderView::D3D11RenderTargetRenderView(Texture& texture, int first_index, int array_size, int level)
		:mRenderTargetSrc(&texture), mRenderTargetFirstSubRes(first_index * texture.getNumMipMaps() + level), mRenderTargetNumSubRes(1)
	{
		mRenderTargetView = checked_cast<D3D11Texture*>(&texture)->retriveD3DRenderTargetView(first_index, array_size, level);
		mWidth = texture.getWidth(level);
		mHeight = texture.getHeight(level);
		mFormat = texture.getFormat();
		this->bindDiscardFunc();
	}
	void D3D11RenderTargetRenderView::discard()
	{
		mDiscardFunc();
	}

	void D3D11RenderTargetRenderView::bindDiscardFunc()
	{
		D3D11RenderEngine& re = *checked_cast<D3D11RenderEngine*>(&Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance());
		if (re.getD3D11RuntimeSubVer() >= 1)
		{
			mDiscardFunc = std::bind(&D3D11RenderTargetRenderView::HWDiscard, this);
		}
		else
		{
			mDiscardFunc = std::bind(&D3D11RenderTargetRenderView::FackDiscard, this);
		}
	}
	void D3D11RenderTargetRenderView::clearColor(Color const &clr)
	{
		mD3DImmContext->ClearRenderTargetView(mRenderTargetView.get(), clr);
	}

	void D3D11RenderTargetRenderView::clearDepth(float depth)
	{
		BOOST_ASSERT(false);
	}
	void D3D11RenderTargetRenderView::clearStencil(int32_t stencil)
	{
		BOOST_ASSERT(false);
	}
	void D3D11RenderTargetRenderView::clearDepthStencil(float depth, int32_t stanecil)
	{
		BOOST_ASSERT(false);
	}

	void D3D11RenderTargetRenderView::HWDiscard()
	{
		mD3DImmContext1->DiscardView(mRenderTargetView.get());
	}
	void D3D11RenderTargetRenderView::FackDiscard()
	{
		float clr[] = { 0, 0, 0, 0 };
		mD3DImmContext->ClearRenderTargetView(mRenderTargetView.get(), clr);
	}

	void D3D11DepthStencilRenderView::bindDiscardFunc()
	{
		D3D11RenderEngine& re = *checked_cast<D3D11RenderEngine*>(&Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance());
		if (re.getD3D11RuntimeSubVer() >= 1)
		{
			mDiscardFunc = std::bind(&D3D11DepthStencilRenderView::HWDiscard, this);
		}
		else
		{
			mDiscardFunc = std::bind(&D3D11DepthStencilRenderView::FackDiscard, this);
		}
	}
	void D3D11DepthStencilRenderView::HWDiscard()
	{
		mD3DImmContext1->DiscardView(mDepthStencilView.get());
	}
	void D3D11DepthStencilRenderView::FackDiscard()
	{
		mD3DImmContext->ClearDepthStencilView(mDepthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	}

	D3D11DepthStencilRenderView::D3D11DepthStencilRenderView(Texture& texture, int first_array_index, int array_size, int level)
		:D3D11RenderView(&texture), mRenderTargetFirstSubRes(first_array_index * texture.getNumMipMaps() + level), mRenderTargetNumSubRes(1)
	{
		mDepthStencilView = checked_cast<D3D11Texture*>(&texture)->retriveD3DDepthStencilView(first_array_index, array_size, level);
		mWidth = texture.getWidth(level);
		mHeight = texture.getHeight(level);
		mFormat = texture.getFormat();
		this->bindDiscardFunc();
	}


	void D3D11DepthStencilRenderView::clearColor(Color const &clr)
	{
		BOOST_ASSERT(false);
	}
	void D3D11DepthStencilRenderView::clearDepth(float depth)
	{
		mD3DImmContext->ClearDepthStencilView(mDepthStencilView.get(), D3D11_CLEAR_DEPTH, depth, 0);
	}
	void D3D11DepthStencilRenderView::clearStencil(int32_t stencil)
	{
		mD3DImmContext->ClearDepthStencilView(mDepthStencilView.get(), D3D11_CLEAR_STENCIL, 1, stencil);
	}

	void D3D11DepthStencilRenderView::clearDepthStencil(float depth, int32_t stanecil)
	{
		mD3DImmContext->ClearDepthStencilView(mDepthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stanecil);
	}

	void D3D11DepthStencilRenderView::discard()
	{
		mDiscardFunc();
	}
}