#ifndef _D3D11_Render_Window_H_
#define _D3D11_Render_Window_H_
#include <boost/signals2.hpp>
#include "D3D11FrameBuffer.hpp"
#include "D3D11Adapter.hpp"
#include "D3D11Typedefs.hpp"
#include "PreDeclare.h"
#pragma once

namespace Air
{
	class D3D11RenderWindow : public D3D11FrameBuffer
	{
	public:
		D3D11RenderWindow(D3D11AdapterPtr const & adapter, std::string const & name, RenderSettings const & settings);
		~D3D11RenderWindow();

		bool isFullScreen() const;

		void setFullScreen(bool fs);

		TexturePtr const & getD3DDepthStencilBuffer() const
		{
			return mDepthStencil;
		}
		TexturePtr const & getD3DBackBuffer() const
		{
			return mBackBuffer;
		}

		D3D11Adapter const & getAdapter() const
		{
			return *mAdapter;
		}

		void swapBuffers();

		void windowMovedOrResized();

	private:
		void onExitSizeMove(Window const &win);
		void onSize(Window const &win, bool active);
		void createSwapChain(ID3D11Device* d3d_device);
		void updateSurfacesPtrs();
	private:
		D3D11AdapterPtr mAdapter;
		HWND mHwnd;
		std::string mName;
		std::wstring mDescription;
		bool mIsFullScreen;
		bool mDXGIStereoSupport;
		bool mDXGIAllowTearing;
		bool mIsMainWnd;
		uint32_t mSyncInterval;
		DXGI_FORMAT mBackBufferFormat;
		ElementFormat mDepthStencilFormat;


#ifdef AIR_PLATFORM_WINDOWS_DESKTOP
		DXGI_SWAP_CHAIN_DESC mSwapChainDesc;
		DXGI_SWAP_CHAIN_DESC1 mSwapChainDesc1;
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC mSwapChainFullScreenDesc;
#endif // AIR_PLATFORM_WINDOWS_DESKTOP


		IDXGISwapChainPtr			mSwapChain;
		IDXGISwapChain1Ptr			mSwapChain1;

		TexturePtr					mBackBuffer;
		TexturePtr					mDepthStencil;
		RenderViewPtr				mRenderTargetView;
		RenderViewPtr				mDepthStencilView;
		RenderViewPtr				mRenderTargetViewRightEye;
		RenderViewPtr				mDepthStencilViewRightEye;

		DWORD mStereoCookie;

		boost::signals2::connection mOnExitSizeMoveConnect;
		boost::signals2::connection mOnSizeConnect;
	};


	typedef std::shared_ptr<D3D11RenderWindow>	D3D11RenderWindowPtr;


}

#endif