#ifndef _D3D11RenderView_H_
#define _D3D11RenderView_H_
#pragma once

#include "rendersystem/include/RenderView.hpp"
#include "rendersystem/include/Viewport.hpp"
#include "D3D11Typedefs.hpp"

namespace Air
{
	class D3D11Texture1D;
	class D3D11Texture2D;
	class D3D11Texture3D;
	class D3D11TextureCube;
	class D3D11GraphicsBuffer;

	class D3D11RenderView : public RenderView
	{
	public:
		D3D11RenderView();
		D3D11RenderView(Texture* texture);
		virtual ~D3D11RenderView();
		virtual void onAttached(FrameBuffer& fb, uint32_t att);
		virtual void onDetached(FrameBuffer& fb, uint32_t att);
	protected:
		ID3D11Device* mD3DDevice;
		ID3D11DeviceContext* mD3DImmContext;
		ID3D11DeviceContext1* mD3DImmContext1;
		void* mSrcTexture;
	};

	class D3D11RenderTargetRenderView : public D3D11RenderView
	{
	public:
		D3D11RenderTargetRenderView(Texture& texture_1d_2d_cube, int first_index, int array_size, int level);
		void clearColor(Color const &clr);
		void clearDepth(float depth);
		void clearStencil(int32_t stencil);
		void clearDepthStencil(float depth, int32_t stanecil);
		virtual void discard() override;

		void onAttached(FrameBuffer& fb, uint32_t att)
		{

		}
		void onDetached(FrameBuffer& fb, uint32_t att)
		{

		}
		ID3D11RenderTargetView* getD3DRenderTargetView() const
		{
			return mRenderTargetView.get();
		}

		void* getRenderTargetSrc() const
		{
			return mRenderTargetSrc;
		}

		uint32_t getRTFirstSubRes() const
		{
			return mRenderTargetFirstSubRes;
		}

		uint32_t getRTNumSubRes() const
		{
			return mRenderTargetNumSubRes;
		}



	private:
		void bindDiscardFunc();
		void HWDiscard();
		void FackDiscard();
	private:
		ID3D11RenderTargetViewPtr mRenderTargetView;
		void* mRenderTargetSrc;
		uint32_t mRenderTargetFirstSubRes;
		uint32_t mRenderTargetNumSubRes;
		std::function<void()> mDiscardFunc;
	};

	class D3D11DepthStencilRenderView : public D3D11RenderView
	{
	public:
		D3D11DepthStencilRenderView(Texture& texture, int first_array_index, int array_size, int level);
		void clearColor(Color const &clr);
		void clearDepth(float depth);
		void clearStencil(int32_t stencil);
		void clearDepthStencil(float depth, int32_t stanecil);
		void onAttached(FrameBuffer& fb, uint32_t att)
		{

		}
		void onDetached(FrameBuffer& fb, uint32_t att)
		{

		}

		void* getRenderTargetSrc() const
		{
			return mRenderTargetSrc;
		}

		uint32_t getRTFirstSubRes() const
		{
			return mRenderTargetFirstSubRes;
		}

		uint32_t getRTNumSubRes() const
		{
			return mRenderTargetNumSubRes;
		}


		ID3D11DepthStencilView* getD3DDepthStencilView() const
		{
			return mDepthStencilView.get();
		}

		virtual void discard() override;
	private:
		void bindDiscardFunc();
		void HWDiscard();
		void FackDiscard();
	private:
		ID3D11DepthStencilViewPtr mDepthStencilView;
		uint32_t mRenderTargetFirstSubRes;
		uint32_t mRenderTargetNumSubRes;
		void* mRenderTargetSrc;
		std::function<void()> mDiscardFunc;
	};
}



#endif