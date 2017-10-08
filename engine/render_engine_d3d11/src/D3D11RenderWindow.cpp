#include "Engine.h"
#include "D3D11RenderView.hpp"
#include <boost/lexical_cast.hpp>
#include "basic/include/ErrorHanding.hpp"
#include "app/include/App3D.hpp"
#include "app/include/Window.hpp"
#include "D3D11FrameBuffer.hpp"
#include "D3D11Mapping.hpp"
#include "D3D11RenderEngine.hpp"
#include "D3D11RenderFactory.hpp"
#include "D3D11Texture.hpp"
#include "D3D11RenderFactoryInternal.hpp"
#include "D3D11RenderWindow.hpp"


namespace Air
{
	D3D11RenderWindow::D3D11RenderWindow(D3D11AdapterPtr const & adapter, std::string const & name, RenderSettings const & settings)
		:mHwnd(nullptr), mAdapter(adapter), mDXGIStereoSupport(false), mDXGIAllowTearing(false)
	{
		mName = name;
		mIsFullScreen = settings.full_screen;
		mSyncInterval = settings.mSyncInterval;
		ElementFormat format = settings.mColorFmt;

		WindowPtr const & main_wnd = Engine::getInstance().getAppInstance().getMainWnd();
		mHwnd = main_wnd->getHWnd();
		mOnExitSizeMoveConnect = main_wnd->onExitSizeMove().connect(std::bind(&D3D11RenderWindow::onExitSizeMove, this, std::placeholders::_1));
		mOnSizeConnect = main_wnd->onSize().connect(std::bind(&D3D11RenderWindow::onSize, this, std::placeholders::_1, std::placeholders::_2));

		if (this->isFullScreen())
		{
			mLeft = 0;
			mTop = 0;
			mWidth = settings.width;
			mHeight = settings.height;
		}
		else
		{
			mLeft = main_wnd->getLeft();
			mTop = main_wnd->getTop();
			mWidth = main_wnd->getWidth();
			mHeight = main_wnd->getHeight();
		}
		mBackBufferFormat = D3D11Mapping::MappingFormat(format);
		mViewport->mLeft = 0;
		mViewport->mTop = 0;
		mViewport->mWidth = mWidth;
		mViewport->mHeight = mHeight;

		RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();
		D3D11RenderEngine& d3d11_re = *checked_cast<D3D11RenderEngine*>(&rf.getRenderEngineInstance());
		ID3D11Device* d3d_device = d3d11_re.getD3DDevice();
		ID3D11DeviceContext* d3d_imm_context = nullptr;
		if (d3d11_re.getDXGISubVer() >= 2)
		{
			mDXGIStereoSupport = d3d11_re.getDXGIFactory2()->IsWindowedStereoEnabled() ? true : false;
		}
		if (d3d11_re.getDXGISubVer() >= 5)
		{
			BOOL allow_tearing = FALSE;
			if (SUCCEEDED(d3d11_re.getDXGIFactory5()->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof(allow_tearing))))
			{
				mDXGIAllowTearing = allow_tearing ? true : false;
			}
		}

		if (d3d_device)
		{
			d3d_imm_context = d3d11_re.getD3DDeviceContext();
			mIsMainWnd = false;
		}
		else
		{
			std::vector<UINT> available_create_device_flags;
			UINT create_device_flags = 0;
#ifdef AIR_DEBUG
			available_create_device_flags.push_back(create_device_flags | D3D11_CREATE_DEVICE_DEBUG);
#endif // DEBUG
			available_create_device_flags.push_back(create_device_flags);
			std::vector<std::pair<D3D_DRIVER_TYPE, wchar_t const *>> dev_type_behaviors;
			dev_type_behaviors.emplace_back(D3D_DRIVER_TYPE_HARDWARE, L"HW");
			dev_type_behaviors.emplace_back(D3D_DRIVER_TYPE_WARP, L"WARP");
			dev_type_behaviors.emplace_back(D3D_DRIVER_TYPE_REFERENCE, L"REF");

			std::vector<std::pair<char const *, D3D_FEATURE_LEVEL>> available_feature_levels;
// 			if (d3d11_re.getDXGISubVer() >= 4)
// 			{
// 				available_feature_levels.emplace_back("12_1", D3D_FEATURE_LEVEL_12_1);
// 				available_feature_levels.emplace_back("12_0", D3D_FEATURE_LEVEL_12_0);
// 			}
// 			if(d3d11_re.getDXGISubVer() >= 2)
// 			{
// 				available_feature_levels.emplace_back("11_1", D3D_FEATURE_LEVEL_11_1);
// 			}
			available_feature_levels.emplace_back("11_0", D3D_FEATURE_LEVEL_11_0);
			available_feature_levels.emplace_back("10_1", D3D_FEATURE_LEVEL_10_1);
			available_feature_levels.emplace_back("10_0", D3D_FEATURE_LEVEL_10_0);
			available_feature_levels.emplace_back("9_3", D3D_FEATURE_LEVEL_9_3);
			available_feature_levels.emplace_back("9_2", D3D_FEATURE_LEVEL_9_2);
			available_feature_levels.emplace_back("9_1", D3D_FEATURE_LEVEL_9_1);

			for (size_t index = 0; index < settings.options.size(); ++index)
			{
				std::string const & opt_name = settings.options[index].first;
				std::string const & opt_val = settings.options[index].second;
				if (0 == strcmp("level", opt_name.c_str()))
				{
					size_t feature_index = 0;
					for (size_t i = 0; i < available_feature_levels.size(); ++i)
					{
						if (0 == strcmp(available_feature_levels[i].first, opt_val.c_str()))
						{
							feature_index = i;
							break;
						}
					}
					if (feature_index > 0)
					{
						available_feature_levels.erase(available_feature_levels.begin(), available_feature_levels.begin() + feature_index);
					}
				}
			}
			std::vector<D3D_FEATURE_LEVEL> feature_levels;
			for (size_t i = 0; i < available_feature_levels.size(); ++i)
			{
				feature_levels.push_back(available_feature_levels[i].second);
			}
			for (auto const & dev_type_beh : dev_type_behaviors)
			{
				d3d_device = nullptr;
				d3d_imm_context = nullptr;
				IDXGIAdapter* dx_adapter = nullptr;
				D3D_DRIVER_TYPE dev_type = dev_type_beh.first;
				if (D3D_DRIVER_TYPE_HARDWARE == dev_type)
				{
					dx_adapter = mAdapter->getAdapter().get();
					dev_type = D3D_DRIVER_TYPE_UNKNOWN;
				}
				D3D_FEATURE_LEVEL out_feature_level = D3D_FEATURE_LEVEL_9_1;
				HRESULT hr = E_FAIL;
				for (auto const & flags : available_create_device_flags)
				{
					ID3D11Device* this_device = nullptr;
					ID3D11DeviceContext* this_imm_context = nullptr;
					D3D_FEATURE_LEVEL this_out_feature_level;
					hr = d3d11_re.d3d11CreateDevice(dx_adapter, dev_type, nullptr, flags, &feature_levels[0], static_cast<UINT>(feature_levels.size()), D3D11_SDK_VERSION, &this_device, &this_out_feature_level, &this_imm_context);
					if (SUCCEEDED(hr))
					{
						d3d_device = this_device;
						d3d_imm_context = this_imm_context;
						out_feature_level = this_out_feature_level;
						break;
					}
				}
				if (SUCCEEDED(hr))
				{
					d3d11_re.setD3DDevice(d3d_device, d3d_imm_context, out_feature_level);
					if (Engine::getInstance().getAppInstance().confirmDevice())
					{
						if (dev_type != D3D_DRIVER_TYPE_HARDWARE)
						{
							IDXGIDevice1* dxgi_device = nullptr;
							hr = d3d_device->QueryInterface(__uuidof(IDXGIDevice1), reinterpret_cast<void**>(&dxgi_device));
							if (SUCCEEDED(hr) && dxgi_device != nullptr)
							{
								IDXGIAdapter* ada;
								dxgi_device->GetAdapter(&ada);
								IDXGIAdapter1* ada1;
								ada->QueryInterface(__uuidof(IDXGIAdapter1), reinterpret_cast<void**>(&ada1));
								ada->Release();
								mAdapter->resetAdapter(MakeComPtr(ada1));
								mAdapter->enumerate();
							}
							dxgi_device->Release();
						}
						mDescription = mAdapter->getDescription() + L" " + dev_type_beh.second + L" FL ";
						wchar_t const * fl_str;
						switch (static_cast<uint32_t>(out_feature_level))
						{
						case D3D_FEATURE_LEVEL_12_1:
							fl_str = L"12.1";
							break;

						case D3D_FEATURE_LEVEL_12_0:
							fl_str = L"12.0";
							break; 
						case D3D_FEATURE_LEVEL_11_1:
							fl_str = L"11.1";
							break; 
						case D3D_FEATURE_LEVEL_11_0:
							fl_str = L"11.0";
							break; 
						case D3D_FEATURE_LEVEL_10_1:
							fl_str = L"10.1";
							break; 
						case D3D_FEATURE_LEVEL_10_0:
							fl_str = L"10.0";
							break; 
						case D3D_FEATURE_LEVEL_9_3:
							fl_str = L"9.3";
							break; 
						case D3D_FEATURE_LEVEL_9_2:
							fl_str = L"9.2";
							break; 
						case D3D_FEATURE_LEVEL_9_1:
							fl_str = L"9.1";
							break;
						default:
							fl_str = L"Unknown";
							break;
						}
						mDescription += fl_str;
						if (settings.mSampleCount > 1)
						{
							mDescription += L" (" + boost::lexical_cast<std::wstring>(settings.mSampleCount)
								+ L"x AA";
						}
						break;
					}
					else
					{
						d3d_device->Release();
						d3d_device = nullptr;
						d3d_imm_context->Release();
						d3d_imm_context = nullptr;
					}
				}
			}
			mIsMainWnd = true;
		}
		Verify(!!d3d_device);
		Verify(!!d3d_imm_context);
		AIR_ASSUME(!!d3d_device);
		AIR_ASSUME(!!d3d_imm_context);

		mDepthStencilFormat = settings.mDepthStencilFmt;
		if (isDepthFormat(mDepthStencilFormat))
		{
			BOOST_ASSERT((EF_D32F == mDepthStencilFormat) || (EF_D24S8 == mDepthStencilFormat) || (EF_D16 == mDepthStencilFormat));
			UINT format_support;
			if (EF_D32F == mDepthStencilFormat)
			{
				d3d_device->CheckFormatSupport(DXGI_FORMAT_D32_FLOAT, &format_support);
				if (!(format_support & D3D11_FORMAT_SUPPORT_DEPTH_STENCIL))
				{
					mDepthStencilFormat = EF_D24S8;
				}
			}
			if (EF_D24S8 == mDepthStencilFormat)
			{
				d3d_device->CheckFormatSupport(DXGI_FORMAT_D24_UNORM_S8_UINT, &format_support);
				if (!(format_support & D3D11_FORMAT_SUPPORT_DEPTH_STENCIL))
				{
					mDepthStencilFormat = EF_D16;
				}
			}
			if (EF_D16 == mDepthStencilFormat)
			{
				d3d_device->CheckFormatSupport(DXGI_FORMAT_D16_UNORM, &format_support);
				if (!(format_support & D3D11_FORMAT_SUPPORT_DEPTH_STENCIL))
				{
					mDepthStencilFormat = EF_Unknown;
				}
			}
		}


#ifdef AIR_PLATFORM_WINDOWS_DESKTOP
		if (d3d11_re.getDXGISubVer() >= 2)
		{
			bool stereo = (STM_LCDShutter == settings.mStereoMethod) && mDXGIStereoSupport;
			d3d11_re.getDXGIFactory2()->RegisterStereoStatusWindow(mHwnd, WM_SIZE, &mStereoCookie);

			mSwapChainDesc1.Width = this->getWidth();
			mSwapChainDesc1.Height = this->getHeight();
			mSwapChainDesc1.Format = mBackBufferFormat;
			mSwapChainDesc1.Stereo = stereo;
			mSwapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			mSwapChainDesc1.BufferCount = 2;
			mSwapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			mSwapChainDesc1.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			if (stereo)
			{
				mSwapChainDesc1.SampleDesc.Count = 1;
				mSwapChainDesc1.SampleDesc.Quality = 0;
				mSwapChainDesc1.Scaling = DXGI_SCALING_NONE;
				mSwapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
			}
			else
			{
				mSwapChainDesc1.SampleDesc.Count = std::min(static_cast<uint32_t>(D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT), settings.mSampleCount);
				mSwapChainDesc1.SampleDesc.Quality = settings.mSampleQuality;
				mSwapChainDesc1.Scaling = DXGI_SCALING_STRETCH;
				if (mDXGIAllowTearing)
				{
					mSwapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
				}
				else
				{
					mSwapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
				}
			}
			if (mDXGIAllowTearing)
			{
				mSwapChainDesc1.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
			}
			mSwapChainFullScreenDesc.RefreshRate.Numerator = 60;
			mSwapChainFullScreenDesc.RefreshRate.Denominator = 1;
			mSwapChainFullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			mSwapChainFullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			mSwapChainFullScreenDesc.Windowed = !this->isFullScreen();
		}
		else
		{
			mSwapChainDesc.BufferDesc.Width = this->getWidth();
			mSwapChainDesc.BufferDesc.Height = this->getHeight();
			mSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			mSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			mSwapChainDesc.BufferDesc.Format = mBackBufferFormat;
			mSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			mSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			mSwapChainDesc.SampleDesc.Count = std::min(static_cast<uint32_t>(D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT), settings.mSampleCount);
			mSwapChainDesc.SampleDesc.Quality = settings.mSampleQuality;
			mSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			mSwapChainDesc.BufferCount = 2;
			mSwapChainDesc.OutputWindow = mHwnd;
			mSwapChainDesc.Windowed = !this->isFullScreen();
			mSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			mSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		}
#endif // AIR_PLATFORM_WINDOWS_DESKTOP

#ifdef AIR_PLATFORM_WINDOWS_DESKTOP
		if ((STM_LCDShutter == settings.mStereoMethod) && !mDXGIStereoSupport)
		{
			if (mAdapter->getDescription().find(L"AMD", 0) != std::wstring::npos)
			{
#ifdef AIR_PLATFORM_WIN64
				const TCHAR* ati_driver = TEXT("atidxx64.dll");
#else
				const TCHAR* ati_driver = TEXT("atidxx32.dll");
#endif // AIR_PLATFORM_WIN64
				HMODULE dll = ::GetModuleHandle(ati_driver);
				if (dll != nullptr)
				{
					//PFNAmdDxExtCreate11
				}

			}
		}
#endif // AIR_PLATFORM_WINDOWS_DESKTOP
		this->createSwapChain(d3d_device);
		Verify(!!mSwapChain);

#ifdef AIR_PLATFORM_WINDOWS_DESKTOP
		d3d11_re.getDXGIFactory1()->MakeWindowAssociation(mHwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
		mSwapChain->SetFullscreenState(this->isFullScreen(), nullptr);
#endif // AIR_PLATFORM_WINDOWS_DESKTOP
		this->updateSurfacesPtrs();
	}
	
	D3D11RenderWindow::~D3D11RenderWindow()
	{

	}

	void D3D11RenderWindow::updateSurfacesPtrs()
	{
		RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();
#if defined AIR_PLATFORM_WINDOWS_DESKTOP
		if (mDXGIAllowTearing)
#endif
		{
			D3D11RenderEngine& d3d11RenderEngine = *checked_cast<D3D11RenderEngine*>(&rf.getRenderEngineInstance());
			if (d3d11RenderEngine.getDXGISubVer() >= 2)
			{
				BOOST_ASSERT(mSwapChain1);
				WindowPtr const & main_wnd = Engine::getInstance().getAppInstance().getMainWnd();
				Window::WindowRotation const rotation = main_wnd->getRotation();

				DXGI_MODE_ROTATION dxgi_rotation;
				switch (rotation)
				{
				case Window::WR_Identity:
					dxgi_rotation = DXGI_MODE_ROTATION_IDENTITY;
					break;
				case Window::WR_Rotate90:
					dxgi_rotation = DXGI_MODE_ROTATION_ROTATE90;
					break;
				case Window::WR_Rotate180:
					dxgi_rotation = DXGI_MODE_ROTATION_ROTATE180;
					break;
				case Window::WR_Rotate270:
					dxgi_rotation = DXGI_MODE_ROTATION_ROTATE270;
					break;
				default:
					BOOST_ASSERT(false);
					dxgi_rotation = DXGI_MODE_ROTATION_UNSPECIFIED;
					break;
				}
				TIFHR(mSwapChain1->SetRotation(dxgi_rotation));
			}
		}
		ID3D11Texture2D* back_buffer;
		TIFHR(mSwapChain1->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer)));
		mBackBuffer = MakeSharedPtr<D3D11Texture2D>(MakeComPtr(back_buffer));
		mRenderTargetView = rf.Make2DRenderView(*mBackBuffer, 0, 1, 0);
		bool stereo = (STM_LCDShutter == Engine::getInstance().getConfig().mGraphicsCfg.mStereoMethod) && mDXGIStereoSupport;
		if (stereo)
		{
			mRenderTargetViewRightEye = rf.Make2DRenderView(*mBackBuffer, 1, 1, 0);
		}
		if (mDepthStencilFormat != EF_Unknown)
		{
			mDepthStencil = rf.MakeTexture2D(mWidth, mHeight, 1, stereo ? 2 : 1, mDepthStencilFormat, mBackBuffer->getSampleCount(), mBackBuffer->getSampleQuality(), EAH_GPU_Read | EAH_GPU_Write);

			mDepthStencilView = rf.Make2DDepthStencilRenderView(*mDepthStencil, 0, 1, 0);
			if (stereo)
			{
				mDepthStencilViewRightEye = rf.Make2DDepthStencilRenderView(*mDepthStencil, 1, 1, 0);
			}
		}
		//TODO amd
		this->attach(ATT_Color0, mRenderTargetView);
		if (mDepthStencilView)
		{
			this->attach(ATT_DepthStencil, mDepthStencilView);
		}
	}

	void D3D11RenderWindow::createSwapChain(ID3D11Device* d3d_device)
	{
		RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();
		D3D11RenderEngine& d3d11_re = *checked_cast<D3D11RenderEngine*>(&rf.getRenderEngineInstance());
		if (d3d11_re.getDXGISubVer() >= 2)
		{
			IDXGISwapChain1* sr1 = nullptr;
			d3d11_re.getDXGIFactory2()->CreateSwapChainForHwnd(d3d_device, mHwnd, &mSwapChainDesc1, &mSwapChainFullScreenDesc, nullptr, &sr1);
			mSwapChain1 = MakeComPtr(sr1);
			IDXGISwapChain* sc;
			mSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&sc));
			mSwapChain = MakeComPtr(sc);
		}
		else
		{
			IDXGISwapChain* sc = nullptr;
			d3d11_re.getDXGIFactory1()->CreateSwapChain(d3d_device, &mSwapChainDesc, &sc);
			mSwapChain = MakeComPtr(sc);
			IDXGISwapChain1* sc1;
			if (SUCCEEDED(mSwapChain->QueryInterface(__uuidof(IDXGISwapChain1), reinterpret_cast<void**>(&sc1))))
			{
				mSwapChain1 = MakeComPtr(sc1);
			}
		}
	}


	void D3D11RenderWindow::windowMovedOrResized()
	{

	}

	void D3D11RenderWindow::onExitSizeMove(Window const &win)
	{
		this->windowMovedOrResized();
	}
	void D3D11RenderWindow::onSize(Window const &win, bool active)
	{
		if (active)
		{
			if (win.getReady())
			{
				this->windowMovedOrResized();
			}
		}
	}

	bool D3D11RenderWindow::isFullScreen() const
	{
		return false;
	}

	void D3D11RenderWindow::setFullScreen(bool fs)
	{

	}

	void D3D11RenderWindow::swapBuffers()
	{
		if (mSwapChain)
		{
			UINT const present_flags = (mDXGIAllowTearing && !mIsFullScreen) ? DXGI_PRESENT_ALLOW_TEARING : 0;
			TIFHR(mSwapChain->Present(mSyncInterval, present_flags));

			RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();
			D3D11RenderEngine& d3d11_re = *checked_cast<D3D11RenderEngine*>(&rf.getRenderEngineInstance());
			if (d3d11_re.getDXGISubVer() >= 2)
			{
				mRenderTargetView->discard();
				if (mDepthStencilView)
				{
					mDepthStencilView->discard();
				}
				if (mRenderTargetViewRightEye)
				{
					mRenderTargetViewRightEye->discard();
				}
				if (mDepthStencilViewRightEye)
				{
					mDepthStencilViewRightEye->discard();
				}
			}
			if (DXGI_PRESENT_ALLOW_TEARING == present_flags)
			{
				d3d11_re.invalidRTVCache();
			}
		}
	}
}