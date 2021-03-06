#ifndef _D3D11RenderView_H_
#define _D3D11RenderView_H_
#pragma once
#include "rendersystem/include/RenderView.hpp"
#include "rendersystem/include/Viewport.hpp"
#include "rendersystem/include/Texture.hpp"
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
		D3D11RenderView(TexturePtr const & texture);
		virtual ~D3D11RenderView();
		virtual void onAttached(FrameBuffer& fb, uint32_t att);
		virtual void onDetached(FrameBuffer& fb, uint32_t att);
	protected:
		ID3D11Device* mD3DDevice;
		ID3D11DeviceContext* mD3DImmContext;
		ID3D11DeviceContext1* mD3DImmContext1;
	};

	class D3D11RenderTargetRenderView : public D3D11RenderView
	{
	public:
		D3D11RenderTargetRenderView(TexturePtr const & texture_1d_2d_cube, int first_index, int array_size, int level);
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
		uint32_t mRenderTargetFirstSubRes;
		uint32_t mRenderTargetNumSubRes;
		std::function<void()> mDiscardFunc;
	};

	class D3D11DepthStencilRenderView : public D3D11RenderView
	{
	public:
		D3D11DepthStencilRenderView(TexturePtr const & texture, int first_array_index, int array_size, int level);
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
		std::function<void()> mDiscardFunc;
	};

	class D3D11UnorderedAccessView : public UnorderAccessView
	{
	public:
		D3D11UnorderedAccessView(TexturePtr & texture, int first_array_index, int array_size, int level);

		D3D11UnorderedAccessView(TexturePtr & texture, int array_index, uint32_t first_slice, uint32_t num_slices, int level);

		D3D11UnorderedAccessView(TexturePtr & texture, int array_index, Texture::CubeFaces face, int level);

		D3D11UnorderedAccessView(GraphicsBuffer& gb, ElementFormat pf);
		virtual ~D3D11UnorderedAccessView();

		void clear(float4 const & val);
		void clear(uint4 const & val);

		virtual void discard() override;

		void onAttached(FrameBuffer& fb, uint32_t att);
		void onDetached(FrameBuffer& fb, uint32_t att);

		ID3D11UnorderedAccessView* getD3DUnorderedAccessView() const
		{
			return mUAView.get();
		}

		void* getUAsrc() const
		{
			return mUASrc;
		}

		uint32_t getUAFirstSubRes() const
		{
			return mUAFirstSubres;
		}

		uint32_t getUANumSubres() const
		{
			return mUANumSubres;
		}

	private:
		void bindDiscardFunc();

		void hwDiscard();

		void fackDisacard();

	private:
		ID3D11Device* mD3DDevice;
		ID3D11DeviceContext* mD3DImmContext;
		ID3D11DeviceContext1* mD3DImmContext1;

		ID3D11UnorderedAccessViewPtr mUAView;
		void* mUASrc;

		uint32_t mUAFirstSubres;
		uint32_t mUANumSubres;

		std::function<void()> mDiscardFunc;
	};
}



#endif