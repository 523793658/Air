#include <dxgi1_5.h>
#include "Engine.h"
#include "basic/include/Util.h"
#include "basic/include/ThrowErr.hpp"
#include "D3D11Typedefs.hpp"
#include "D3D11RenderFactory.hpp"
#include "D3D11RenderWindow.hpp"
#include "D3D11RenderEngine.hpp"

namespace Air
{
	D3D11RenderEngine::D3D11RenderEngine()

	{
		mNativeShaderVersion = 5;
#ifdef AIR_PLATFORM_WINDOWS_DESKTOP
		mModDXGI = ::LoadLibraryEx(TEXT("dxgi.dll"), nullptr, 0);
		if (nullptr == mModDXGI)
		{
			::MessageBoxW(nullptr, L"Can't load dxgi.dll", L"Error", MB_OK);
		}
		mModD3D11 = ::LoadLibraryEx(TEXT("d3d11.dll"), nullptr, 0);
		if (nullptr == mModD3D11)
		{
			::MessageBoxW(nullptr, L"Can't load d3d11.dll", L"Error", MB_OK);
		}
		if (mModDXGI != nullptr)
		{
			mDynamicCreateDXGIFactory1 = reinterpret_cast<CreateDXGIFactory1Func>(::GetProcAddress(mModDXGI, "CreateDXGIFactory1"));
		}
		if (mModD3D11 != nullptr)
		{
			mDynamicD3D11CreateDevice = reinterpret_cast<D3D11CreateDeviceFunc>(::GetProcAddress(mModD3D11, "D3D11CreateDevice"));
		}
#else
		mDynamicCreateDXGIFactory1 = ::CreateDXGIFactory1;
		mDynamicD3D11CreateDevice = ::D3D11CreateDevice;
#endif
		IDXGIFactory1* gi_factory;
		TIF(mDynamicCreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&gi_factory)));
		mGIFactory1 = MakeComPtr(gi_factory);
		mDXGISubVersion = 1;

		IDXGIFactory2* gi_factory2;
		gi_factory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&gi_factory2));
		if (gi_factory2 != nullptr)
		{
			mGIFactory2 = MakeComPtr(gi_factory2);
			mDXGISubVersion = 2;

			IDXGIFactory3* gi_factor3;
			gi_factory->QueryInterface(__uuidof(IDXGIFactory3), reinterpret_cast<void**>(&gi_factor3));
			if (gi_factor3 != nullptr)
			{
				mGIFactory3 = MakeComPtr(gi_factor3);
				mDXGISubVersion = 3;

				IDXGIFactory4* gi_factory4;
				gi_factory->QueryInterface(__uuidof(IDXGIFactory4), reinterpret_cast<void**>(&gi_factory4));
				if (gi_factory4 != nullptr)
				{
					mGIFactory4 = MakeComPtr(gi_factory4);
					mDXGISubVersion = 4;

					IDXGIFactory5* gi_factory5;
					gi_factory->QueryInterface(__uuidof(IDXGIFactory5), reinterpret_cast<void**>(&gi_factory5));
					if (gi_factory5 != nullptr)
					{
						mGIFactory5 = MakeComPtr(gi_factory5);
						mDXGISubVersion = 5;
					}
				}
			}
		}
		mAdapterList.enumerate(mGIFactory1);
	}

	D3D11RenderEngine::~D3D11RenderEngine()
	{
		this->destroy();
	}

	std::wstring const & D3D11RenderEngine::getName() const
	{
		static std::wstring const name(L"Direct3D11 Render Engine");
		return name;
	}

	void D3D11RenderEngine::beginFrame()
	{
		if ((mD3DFeatureLevel >= D3D_FEATURE_LEVEL_10_0) && Engine::getInstance().getConfig().mPerfProfiler)
		{
			mD3DIMMContext->Begin(mTimestampDisJointQuery.get());
		}
		RenderEngine::beginFrame();
	}
	void D3D11RenderEngine::endFrame()
	{
		RenderEngine::endFrame();
		if ((mD3DFeatureLevel >= D3D_FEATURE_LEVEL_10_0) && Engine::getInstance().getConfig().mPerfProfiler)
		{
			mD3DIMMContext->End(mTimestampDisJointQuery.get());
		}
	}


	void D3D11RenderEngine::updateGPUTimestampsFrequency()
	{
		if (mD3DFeatureLevel >= D3D_FEATURE_LEVEL_10_0)
		{
			D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjoint;
			while (S_OK != mD3DIMMContext->GetData(mTimestampDisJointQuery.get(), &disjoint, sizeof(disjoint), 0));
			mInvTimestampFreq = disjoint.Disjoint ? 0 : (1.0 / disjoint.Frequency);
		}
	}

	IDXGIFactory1* D3D11RenderEngine::getDXGIFactory1() const
	{
		return mGIFactory1.get();
	}
	IDXGIFactory2* D3D11RenderEngine::getDXGIFactory2() const
	{
		return mGIFactory2.get();
	}
	IDXGIFactory3* D3D11RenderEngine::getDXGIFactory3() const
	{
		return mGIFactory3.get();
	}
	IDXGIFactory4* D3D11RenderEngine::getDXGIFactory4() const
	{
		return mGIFactory4.get();
	}
	IDXGIFactory5* D3D11RenderEngine::getDXGIFactory5() const
	{
		return mGIFactory5.get();
	}



	uint8_t D3D11RenderEngine::getDXGISubVer() const
	{
		return mDXGISubVersion;
	}

	ID3D11Device* D3D11RenderEngine::getD3DDevice() const
	{
		return mD3D11Device.get();
	}
	ID3D11Device1* D3D11RenderEngine::getD3DDevice1() const
	{
		return mD3D11Device1.get();
	}
	ID3D11Device2* D3D11RenderEngine::getD3DDevice2() const
	{
		return mD3D11Device2.get();
	}
	ID3D11Device3* D3D11RenderEngine::getD3DDevice3() const
	{
		return mD3D11Device3.get();
	}

	ID3D11DeviceContext* D3D11RenderEngine::getD3DDeviceContext() const
	{
		return mD3DIMMContext.get();
	}
	ID3D11DeviceContext1* D3D11RenderEngine::getD3DDeviceContext1() const
	{
		return mD3DIMMContext1.get();
	}
	ID3D11DeviceContext2* D3D11RenderEngine::getD3DDeviceContext2() const
	{
		return mD3DIMMContext2.get();
	}
	ID3D11DeviceContext3* D3D11RenderEngine::getD3DDeviceContext3() const
	{
		return mD3DIMMContext3.get();
	}

	uint8_t D3D11RenderEngine::getD3D11RuntimeSubVer() const
	{
		return mD3D11RuntimeSubVer;
	}

	D3D_FEATURE_LEVEL D3D11RenderEngine::getDeviceFeatureLevel() const
	{
		return mD3DFeatureLevel;
	}

	void D3D11RenderEngine::setD3DDevice(ID3D11Device* device, ID3D11DeviceContext* context, D3D_FEATURE_LEVEL feature_level)
	{
		this->detectD3D11Runtime(device, context);
		mD3DFeatureLevel = feature_level;
		Verify(device != nullptr);
		this->fillRenderDeviceCaps();
	}

	void D3D11RenderEngine::forceFlush()
	{
		mD3DIMMContext->Flush();
	}

	void D3D11RenderEngine::destroy()
	{

	}


	bool D3D11RenderEngine::isFullScreen() const
	{
		return checked_cast<D3D11RenderWindow*>(mScreenFrameBuffer.get())->isFullScreen();
	}
	void D3D11RenderEngine::setFullScreen(bool fs)
	{
		checked_cast<D3D11RenderWindow*>(mScreenFrameBuffer.get())->setFullScreen(fs);
	}

	void D3D11RenderEngine::detectD3D11Runtime(ID3D11Device* device, ID3D11DeviceContext* imm_ctx)
	{
		mD3D11Device = MakeComPtr(device);
		mD3DIMMContext = MakeComPtr(imm_ctx);
		mD3D11RuntimeSubVer = 0;

		ID3D11Device1* d3d_device1 = nullptr;
		device->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&d3d_device1));
		if (d3d_device1)
		{
			ID3D11DeviceContext1* d3d_imm_ctx_1 = nullptr;
			imm_ctx->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&d3d_imm_ctx_1));
			if (d3d_imm_ctx_1)
			{
				mD3DIMMContext1 = MakeComPtr(d3d_imm_ctx_1);
				mD3D11Device1 = MakeComPtr(d3d_device1);
				mD3D11RuntimeSubVer = 1;

				ID3D11Device2* d3d_device2 = nullptr;
				device->QueryInterface(__uuidof(ID3D11Device2), reinterpret_cast<void**>(&d3d_device2));
				if (d3d_device2)
				{
					ID3D11DeviceContext2* d3d_imm_ctx2 = nullptr;
					imm_ctx->QueryInterface(__uuidof(ID3D11DeviceContext2), reinterpret_cast<void**>(&d3d_imm_ctx2));
					if (d3d_imm_ctx2)
					{
						mD3D11Device2 = MakeComPtr(d3d_device2);
						mD3DIMMContext2 = MakeComPtr(d3d_imm_ctx2);
						mD3D11RuntimeSubVer = 2;

						ID3D11Device3* d3d_device3 = nullptr;
						device->QueryInterface(__uuidof(ID3D11Device3), reinterpret_cast<void**>(&d3d_device3));
						if (d3d_device3)
						{
							ID3D11DeviceContext3* d3d_context3 = nullptr;
							imm_ctx->QueryInterface(__uuidof(ID3D11DeviceContext3), reinterpret_cast<void**>(&d3d_context3));
							if (d3d_context3)
							{
								mD3D11Device3 = MakeComPtr(d3d_device3);
								mD3DIMMContext3 = MakeComPtr(d3d_context3);
								mD3D11RuntimeSubVer = 3;
							}
							else
							{
								d3d_device3->Release();
								d3d_device3 = nullptr;
							}
						}
					}
					else
					{
						d3d_device2->Release();
						d3d_device2 = nullptr;
					}
				}
			}

			else
			{
				d3d_device1->Release();
				d3d_device1 = nullptr;
			}
		}
	}



	void D3D11RenderEngine::fillRenderDeviceCaps()
	{
		BOOST_ASSERT(mD3D11Device);
		switch (mD3DFeatureLevel)
		{
		case D3D_FEATURE_LEVEL_9_1:
		case D3D_FEATURE_LEVEL_9_2:
			mCaps.mMaxShaderModel = ShaderModel(2, 0);
			mCaps.mMaxTextureWidth = mCaps.mMaxTextureHeight = D3D_FL9_1_REQ_TEXTURE2D_U_OR_V_DIMENSION;
			mCaps.mMaxTextureDepth = D3D_FL9_1_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;
			mCaps.mMaxTextureCubeSize = D3D_FL9_1_REQ_TEXTURECUBE_DIMENSION;
			mCaps.mMaxTextureArrayLength = 1;
			mCaps.mMaxVertexTextureUnits = 0;
			mCaps.mMaxPixelTextureUnits = 16;
			mCaps.mMaxGeometryTextureUntis = 0;
			mCaps.mMaxSimultaneousRts = D3D_FL9_1_SIMULTANEOUS_RENDER_TARGET_COUNT;
			mCaps.mMaxSimultaneousUavs = 0;
			mCaps.mCSSupport = false;
			mCaps.mTessMethod = TM_No;
			break;
		case D3D_FEATURE_LEVEL_9_3:
			break;
		case D3D_FEATURE_LEVEL_10_0:
		case D3D_FEATURE_LEVEL_10_1:
			if (mD3DFeatureLevel == D3D_FEATURE_LEVEL_10_1)
			{
				mCaps.mMaxShaderModel = ShaderModel(4, 1);
			}
			else
			{
				mCaps.mMaxShaderModel = ShaderModel(4, 0);
			}
			mCaps.mMaxTextureWidth = mCaps.mMaxTextureHeight = D3D10_REQ_TEXTURE2D_U_OR_V_DIMENSION;
			mCaps.mMaxTextureDepth = D3D10_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;
			mCaps.mMaxTextureCubeSize = D3D10_REQ_TEXTURECUBE_DIMENSION;
			mCaps.mMaxTextureArrayLength = D3D10_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION;
			mCaps.mMaxVertexTextureUnits = D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT;
			mCaps.mMaxPixelTextureUnits = D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT;
			mCaps.mMaxGeometryTextureUntis = D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT;
			mCaps.mMaxSimultaneousRts = D3D10_SIMULTANEOUS_RENDER_TARGET_COUNT;
			mCaps.mMaxSimultaneousUavs = 0;
			{
				D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS cs4_feature;
				mD3D11Device->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &cs4_feature, sizeof(cs4_feature));
				mCaps.mCSSupport= cs4_feature.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x ? true : false;
			}
			mCaps.mTessMethod= TM_No;
			break;
		case D3D_FEATURE_LEVEL_11_0:
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_12_0:
		case D3D_FEATURE_LEVEL_12_1:
			mCaps.mMaxShaderModel = (mD3DFeatureLevel > D3D_FEATURE_LEVEL_12_0) ? ShaderModel(5, 1) : ShaderModel(5, 0);
			mCaps.mMaxTextureWidth = mCaps.mMaxTextureHeight = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
			mCaps.mMaxTextureDepth = D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;
			mCaps.mMaxTextureCubeSize = D3D11_REQ_TEXTURECUBE_DIMENSION;
			mCaps.mMaxTextureArrayLength = D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION;
			mCaps.mMaxVertexTextureUnits = D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT;
			mCaps.mMaxPixelTextureUnits = D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT;
			mCaps.mMaxGeometryTextureUntis = D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT;
			mCaps.mMaxSimultaneousRts = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;
			mCaps.mMaxSimultaneousUavs = D3D11_PS_CS_UAV_REGISTER_COUNT;
			mCaps.mCSSupport = true;
			mCaps.mTessMethod = TM_Hardware;
			break;
		default:
			break;
		}

		switch (mD3DFeatureLevel)
		{
		case D3D_FEATURE_LEVEL_9_1:
		case D3D_FEATURE_LEVEL_9_2:
		case D3D_FEATURE_LEVEL_9_3:
			mCaps.mMaxVertexStreams = 16;
			break;
		case D3D_FEATURE_LEVEL_10_0:
			mCaps.mMaxVertexStreams = D3D10_STANDARD_VERTEX_ELEMENT_COUNT;
			break;
		case D3D_FEATURE_LEVEL_10_1:
			mCaps.mMaxVertexStreams = D3D10_1_STANDARD_VERTEX_ELEMENT_COUNT;
			break;
		case D3D_FEATURE_LEVEL_11_0:
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_12_0:
		case D3D_FEATURE_LEVEL_12_1:
			mCaps.mMaxVertexStreams = D3D11_STANDARD_VERTEX_ELEMENT_COUNT;
			break;
		default:
			break;
		}

		switch (mD3DFeatureLevel)
		{
		case D3D_FEATURE_LEVEL_9_1:
			mCaps.mMaxTextureAnisotropy = 2;
			break;
		case D3D_FEATURE_LEVEL_9_2:
		case D3D_FEATURE_LEVEL_9_3:
			mCaps.mMaxTextureAnisotropy = 10;
			break;
		case D3D_FEATURE_LEVEL_10_0:
		case D3D_FEATURE_LEVEL_10_1:
			mCaps.mMaxTextureAnisotropy = D3D10_MAX_MAXANISOTROPY;
			break;
		case D3D_FEATURE_LEVEL_11_0:
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_12_0:
		case D3D_FEATURE_LEVEL_12_1:
			mCaps.mMaxTextureAnisotropy = D3D11_MAX_MAXANISOTROPY;
			break;
		default:
			break;
		}
		if (mD3D11RuntimeSubVer >= 1)
		{
			D3D11_FEATURE_DATA_ARCHITECTURE_INFO arch_feature;
			mD3D11Device->CheckFeatureSupport(D3D11_FEATURE_ARCHITECTURE_INFO, &arch_feature, sizeof(arch_feature));
			mCaps.mIstbdr = arch_feature.TileBasedDeferredRenderer ? true : false;
		}
		else
		{
			mCaps.mIstbdr = false;
		}
		if (mD3D11RuntimeSubVer >= 2)
		{
			D3D11_FEATURE_DATA_D3D9_SIMPLE_INSTANCING_SUPPORT d3d11_feature;
			mD3D11Device->CheckFeatureSupport(D3D11_FEATURE_D3D9_SIMPLE_INSTANCING_SUPPORT, &d3d11_feature, sizeof(d3d11_feature));
			mCaps.mHWInstancingSupport = d3d11_feature.SimpleInstancingSupported ? true : false;
		}
		else
		{
			mCaps.mHWInstancingSupport = (mD3DFeatureLevel >= D3D_FEATURE_LEVEL_9_3);
		}
		mCaps.mInstanceIdSupport = (mD3DFeatureLevel >= D3D_FEATURE_LEVEL_10_0);
		mCaps.mStreamOuputSupport = (mD3DFeatureLevel >= D3D_FEATURE_LEVEL_10_0);
		mCaps.mAlphaToCoverageSupport = (mD3DFeatureLevel >= D3D_FEATURE_LEVEL_10_0);
		mCaps.mPrimitiveRestartSupport = (mD3DFeatureLevel >= D3D_FEATURE_LEVEL_10_0);
		{
			D3D11_FEATURE_DATA_THREADING mt_feature;
			mD3D11Device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &mt_feature, sizeof(mt_feature));
			mCaps.mMultithreadRenderingSupport = mt_feature.DriverCommandLists ? true : false;
			mCaps.mMultithreadResCreatingSupport = mt_feature.DriverConcurrentCreates ? true : false;
		}
		mCaps.mMRTIndependentBitDepthSupport = (mD3DFeatureLevel >= D3D_FEATURE_LEVEL_10_0);
		mCaps.mStandarDerivativesSupport = (mD3DFeatureLevel >= D3D_FEATURE_LEVEL_9_3);
		mCaps.mShaderTextureLodSupport = (mD3DFeatureLevel >= D3D_FEATURE_LEVEL_10_0);
		if (mD3D11RuntimeSubVer >= 1)
		{
			D3D11_FEATURE_DATA_D3D11_OPTIONS d3d11_feature;
			mD3D11Device->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS, &d3d11_feature, sizeof(d3d11_feature));
			mCaps.mLogicOpSupport = d3d11_feature.OutputMergerLogicOp ? true : false;
		}
		else
		{
			mCaps.mLogicOpSupport = false;
		}
		mCaps.mIndependentBlendSupport = (mD3DFeatureLevel>= D3D_FEATURE_LEVEL_10_0);
		mCaps.mDrawIndirectSupport = (mD3DFeatureLevel >= D3D_FEATURE_LEVEL_11_0);
		mCaps.mNoOverwriteSupport = true;


		if (mD3D11RuntimeSubVer >= 1)
		{
			D3D11_FEATURE_DATA_D3D9_OPTIONS d3d11_feature;
			mD3D11Device->CheckFeatureSupport(D3D11_FEATURE_D3D9_OPTIONS, &d3d11_feature, sizeof(d3d11_feature));
			mCaps.mFullNoptTextureSupport = d3d11_feature.FullNonPow2TextureSupport ? true : false;
		}
		else
		{
			mCaps.mFullNoptTextureSupport= false;
		}
		mCaps.mRenderToTextureArraySupport = (mD3DFeatureLevel>= D3D_FEATURE_LEVEL_10_0);
		mCaps.mLoadFromBufferSupport = (mD3DFeatureLevel >= D3D_FEATURE_LEVEL_10_0);
		mCaps.mGSSupport = (mD3DFeatureLevel>= D3D_FEATURE_LEVEL_10_0);
		mCaps.mHSSupport = (mD3DFeatureLevel>= D3D_FEATURE_LEVEL_11_0);
		mCaps.mDSSupport = (mD3DFeatureLevel>= D3D_FEATURE_LEVEL_11_0);

		bool check_16bpp_fmts = (mDXGISubVersion >= 2);

		std::pair<ElementFormat, DXGI_FORMAT> fmts[] =
		{
			std::make_pair(EF_A8, DXGI_FORMAT_A8_UNORM),
			std::make_pair(EF_R5G6B5, DXGI_FORMAT_B5G6R5_UNORM),
			std::make_pair(EF_A1RGB5, DXGI_FORMAT_B5G5R5A1_UNORM),
			std::make_pair(EF_ARGB4, DXGI_FORMAT_B4G4R4A4_UNORM),
			std::make_pair(EF_R8, DXGI_FORMAT_R8_UNORM),
			std::make_pair(EF_SIGNED_R8, DXGI_FORMAT_R8_SNORM),
			std::make_pair(EF_GR8, DXGI_FORMAT_R8G8_UNORM),
			std::make_pair(EF_SIGNED_GR8, DXGI_FORMAT_R8G8_SNORM),
			std::make_pair(EF_ARGB8, DXGI_FORMAT_B8G8R8A8_UNORM),
			std::make_pair(EF_ABGR8, DXGI_FORMAT_R8G8B8A8_UNORM),
			std::make_pair(EF_SIGNED_ABGR8, DXGI_FORMAT_R8G8B8A8_SNORM),
			std::make_pair(EF_A2BGR10, DXGI_FORMAT_R10G10B10A2_UNORM),
			std::make_pair(EF_SIGNED_A2BGR10, DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM),
			std::make_pair(EF_R8UI, DXGI_FORMAT_R8_UINT),
			std::make_pair(EF_R8I, DXGI_FORMAT_R8_SINT),
			std::make_pair(EF_GR8UI, DXGI_FORMAT_R8G8_UINT),
			std::make_pair(EF_GR8I, DXGI_FORMAT_R8G8_SINT),
			std::make_pair(EF_ABGR8UI, DXGI_FORMAT_R8G8B8A8_UINT),
			std::make_pair(EF_ABGR8I, DXGI_FORMAT_R8G8B8A8_SINT),
			std::make_pair(EF_A2BGR10UI, DXGI_FORMAT_R10G10B10A2_UINT),
			std::make_pair(EF_R16, DXGI_FORMAT_R16_UNORM),
			std::make_pair(EF_SIGNED_R16, DXGI_FORMAT_R16_SNORM),
			std::make_pair(EF_GR16, DXGI_FORMAT_R16G16_UNORM),
			std::make_pair(EF_SIGNED_GR16, DXGI_FORMAT_R16G16_SNORM),
			std::make_pair(EF_ABGR16, DXGI_FORMAT_R16G16B16A16_UNORM),
			std::make_pair(EF_SIGNED_ABGR16, DXGI_FORMAT_R16G16B16A16_SNORM),
			std::make_pair(EF_R16UI, DXGI_FORMAT_R16_UINT),
			std::make_pair(EF_R16I, DXGI_FORMAT_R16_SINT),
			std::make_pair(EF_GR16UI, DXGI_FORMAT_R16G16_UINT),
			std::make_pair(EF_GR16I, DXGI_FORMAT_R16G16_SINT),
			std::make_pair(EF_ABGR16UI, DXGI_FORMAT_R16G16B16A16_UINT),
			std::make_pair(EF_ABGR16I, DXGI_FORMAT_R16G16B16A16_SINT),
			std::make_pair(EF_R32UI, DXGI_FORMAT_R32_UINT),
			std::make_pair(EF_R32I, DXGI_FORMAT_R32_SINT),
			std::make_pair(EF_GR32UI, DXGI_FORMAT_R32G32_UINT),
			std::make_pair(EF_GR32I, DXGI_FORMAT_R32G32_SINT),
			std::make_pair(EF_BGR32UI, DXGI_FORMAT_R32G32B32_UINT),
			std::make_pair(EF_BGR32I, DXGI_FORMAT_R32G32B32_SINT),
			std::make_pair(EF_ABGR32UI, DXGI_FORMAT_R32G32B32A32_UINT),
			std::make_pair(EF_ABGR32I, DXGI_FORMAT_R32G32B32A32_SINT),
			std::make_pair(EF_R16F, DXGI_FORMAT_R16_FLOAT),
			std::make_pair(EF_GR16F, DXGI_FORMAT_R16G16_FLOAT),
			std::make_pair(EF_B10G11R11F, DXGI_FORMAT_R11G11B10_FLOAT),
			std::make_pair(EF_ABGR16F, DXGI_FORMAT_R16G16B16A16_FLOAT),
			std::make_pair(EF_R32F, DXGI_FORMAT_R32_FLOAT),
			std::make_pair(EF_GR32F, DXGI_FORMAT_R32G32_FLOAT),
			std::make_pair(EF_BGR32F, DXGI_FORMAT_R32G32B32_FLOAT),
			std::make_pair(EF_ABGR32F, DXGI_FORMAT_R32G32B32A32_FLOAT),
			std::make_pair(EF_BC1, DXGI_FORMAT_BC1_UNORM),
			std::make_pair(EF_BC2, DXGI_FORMAT_BC2_UNORM),
			std::make_pair(EF_BC3, DXGI_FORMAT_BC3_UNORM),
			std::make_pair(EF_BC4, DXGI_FORMAT_BC4_UNORM),
			std::make_pair(EF_SIGNED_BC4, DXGI_FORMAT_BC4_SNORM),
			std::make_pair(EF_BC5, DXGI_FORMAT_BC5_UNORM),
			std::make_pair(EF_SIGNED_BC5, DXGI_FORMAT_BC5_SNORM),
			std::make_pair(EF_BC6, DXGI_FORMAT_BC6H_UF16),
			std::make_pair(EF_SIGNED_BC6, DXGI_FORMAT_BC6H_SF16),
			std::make_pair(EF_BC7, DXGI_FORMAT_BC7_UNORM),
			std::make_pair(EF_D16, DXGI_FORMAT_D16_UNORM),
			std::make_pair(EF_D24S8, DXGI_FORMAT_D24_UNORM_S8_UINT),
			std::make_pair(EF_D32F, DXGI_FORMAT_D32_FLOAT),
			std::make_pair(EF_ARGB8_SRGB, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB),
			std::make_pair(EF_ABGR8_SRGB, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB),
			std::make_pair(EF_BC1_SRGB, DXGI_FORMAT_BC1_UNORM_SRGB),
			std::make_pair(EF_BC2_SRGB, DXGI_FORMAT_BC2_UNORM_SRGB),
			std::make_pair(EF_BC3_SRGB, DXGI_FORMAT_BC3_UNORM_SRGB),
			std::make_pair(EF_BC7_SRGB, DXGI_FORMAT_BC7_UNORM_SRGB)
		};

		UINT s;
		for (size_t i = 0; i < sizeof(fmts) / sizeof(fmts[0]); ++i)
		{
			if ((mCaps.mMaxShaderModel< ShaderModel(5, 0))
				&& ((EF_BC6 == fmts[i].first) || (EF_SIGNED_BC6 == fmts[i].first)
					|| (EF_BC7 == fmts[i].first) || (EF_BC7_SRGB == fmts[i].first)))
			{
				continue;
			}

			if (!check_16bpp_fmts
				&& ((EF_R5G6B5 == fmts[i].first) || (EF_A1RGB5 == fmts[i].first) || (EF_ARGB4 == fmts[i].first)))
			{
				continue;
			}

			mD3D11Device->CheckFormatSupport(fmts[i].second, &s);
			if (s != 0)
			{
				if (isDepthFormat(fmts[i].first))
				{
					DXGI_FORMAT depth_fmt;
					switch (fmts[i].first)
					{
					case EF_D16:
						depth_fmt = DXGI_FORMAT_R16_TYPELESS;
						break;

					case EF_D24S8:
						depth_fmt = DXGI_FORMAT_R24G8_TYPELESS;
						break;

					case EF_D32F:
					default:
						depth_fmt = DXGI_FORMAT_R32_TYPELESS;
						break;
					}

					UINT s1;
					mD3D11Device->CheckFormatSupport(depth_fmt, &s1);
					if (s1 != 0)
					{
						if (s1 & D3D11_FORMAT_SUPPORT_IA_VERTEX_BUFFER)
						{
							mVertexFormats.insert(fmts[i].first);
						}
						if (s1 & (D3D11_FORMAT_SUPPORT_TEXTURE1D | D3D11_FORMAT_SUPPORT_TEXTURE2D
							| D3D11_FORMAT_SUPPORT_TEXTURE3D | D3D11_FORMAT_SUPPORT_TEXTURECUBE
							| D3D11_FORMAT_SUPPORT_SHADER_LOAD | D3D11_FORMAT_SUPPORT_SHADER_SAMPLE))
						{
							mTextureFormats.insert(fmts[i].first);
						}
					}
				}
				else
				{
					if (s & D3D11_FORMAT_SUPPORT_IA_VERTEX_BUFFER)
					{
						mVertexFormats.insert(fmts[i].first);
					}
					if ((s & (D3D11_FORMAT_SUPPORT_TEXTURE1D | D3D11_FORMAT_SUPPORT_TEXTURE2D
						| D3D11_FORMAT_SUPPORT_TEXTURE3D | D3D11_FORMAT_SUPPORT_TEXTURECUBE))
						&& (s & D3D11_FORMAT_SUPPORT_SHADER_SAMPLE))
					{
						mTextureFormats.insert(fmts[i].first);
					}
				}

				if (s & (D3D11_FORMAT_SUPPORT_RENDER_TARGET | D3D11_FORMAT_SUPPORT_MULTISAMPLE_RENDERTARGET
					| D3D11_FORMAT_SUPPORT_DEPTH_STENCIL))
				{
					UINT count = 1;
					UINT quality;
					while (count <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT)
					{
						if (SUCCEEDED(mD3D11Device->CheckMultisampleQualityLevels(fmts[i].second, count, &quality)))
						{
							if (quality > 0)
							{
								mRendertargetFormats[fmts[i].first].emplace_back(count, quality);
								count <<= 1;
							}
							else
							{
								break;
							}
						}
						else
						{
							break;
						}
					}
				}
			}
		}

		mCaps.vertexFormatSupport = std::bind<bool>(&D3D11RenderEngine::isVertexFormatSupport, this,
			std::placeholders::_1);
		mCaps.textureFormatSupport = std::bind<bool>(&D3D11RenderEngine::isTextureFormatSupport, this,
			std::placeholders::_1);
		mCaps.rendertargetFormatSupport = std::bind<bool>(&D3D11RenderEngine::isRenderTargetFormatSupport, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		mCaps.mDepthTextureSupport = (mCaps.textureFormatSupport(EF_D24S8) || mCaps.textureFormatSupport(EF_D16));
		mCaps.mFPColorSupport = ((mCaps.textureFormatSupport(EF_B10G11R11F) && mCaps.rendertargetFormatSupport(EF_B10G11R11F, 1, 0))
			|| (mCaps.textureFormatSupport(EF_ABGR16F) && mCaps.rendertargetFormatSupport(EF_ABGR16F, 1, 0)));
		mCaps.mPackToRGBARequired = !(mCaps.textureFormatSupport(EF_R16F) && mCaps.rendertargetFormatSupport(EF_R16F, 1, 0)
			&& mCaps.textureFormatSupport(EF_R32F) && mCaps.rendertargetFormatSupport(EF_R32F, 1, 0));
	}

	bool D3D11RenderEngine::isVertexFormatSupport(ElementFormat elem_fmt)
	{
		return mVertexFormats.find(elem_fmt) != mVertexFormats.end();
	}
	bool D3D11RenderEngine::isTextureFormatSupport(ElementFormat elem_fmt)
	{
		return mTextureFormats.find(elem_fmt) != mTextureFormats.end();
	}
	bool D3D11RenderEngine::isRenderTargetFormatSupport(ElementFormat elem_fmt, uint32_t sample_count, uint32_t sample_quality)
	{
		auto iter = mRendertargetFormats.find(elem_fmt);
		if (iter != mRendertargetFormats.end())
		{
			for (auto const & p : iter->second)
			{
				if ((sample_count == p.first) && (sample_quality == p.second))
				{
					return true;
				}
			}
		}
		return false;
	}

	D3D11AdapterPtr const & D3D11RenderEngine::getActiveAdapter() const
	{
		return mAdapterList.getAdapter(mAdapterList.getCurrentAdapterIndex());
	}

	void D3D11RenderEngine::invalidRTVCache()
	{
		mRenderTargetViewPtrChache.clear();
	}

	void D3D11RenderEngine::doCreateRenderWindow(std::string const & name, RenderSettings const & settings)
	{
		mMotionFrames = settings.mMotionFrames;
		D3D11RenderWindowPtr win = MakeSharedPtr<D3D11RenderWindow>(this->getActiveAdapter(), name, settings);
		switch (mD3DFeatureLevel)
		{
		case D3D_FEATURE_LEVEL_9_3:
			mNativeShaderPlatformName = "d3d_9_3";
			mVSProfile = "vs_4_0_level_9_3";
			mPSProfile = "ps_4_0_level_9_3";
			mGSProfile = "";
			mCSProfile = "";
			mHSProfile = "";
			mDSProfile = "";
			break;
		case D3D_FEATURE_LEVEL_10_0:
			mNativeShaderPlatformName = "d3d_10_0";
			mVSProfile = "vs_4_0";
			mPSProfile = "ps_4_0";
			mGSProfile = "gs_4_0";
			mCSProfile = "cs_4_0";
			mHSProfile = "";
			mDSProfile = "";
			break;
		case D3D_FEATURE_LEVEL_10_1:
			mNativeShaderPlatformName = "d3d_10_1";
			mVSProfile = "vs_4_1";
			mPSProfile = "ps_4_1";
			mGSProfile = "gs_4_1";
			mCSProfile = "cs_4_1";
			mHSProfile = "";
			mDSProfile = "";
			break;
		case D3D_FEATURE_LEVEL_11_0:
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_12_0:
		case D3D_FEATURE_LEVEL_12_1:
			mNativeShaderPlatformName = "d3d_11_0";
			mVSProfile = "vs_5_0";
			mPSProfile = "ps_5_0";
			mGSProfile = "gs_5_0";
			mCSProfile = "cs_5_0";
			mHSProfile = "hs_5_0";
			mDSProfile = "ds_5_0";
			break;
		default:
			mNativeShaderPlatformName = "d3d_9_1";
			mVSProfile = "vs_4_0_level_9_1";
			mPSProfile = "ps_4_0_level_9_1";
			mGSProfile = "";
			mCSProfile = "";
			mHSProfile = "";
			mDSProfile = "";
			break;
		}
		this->resetRenderStates();
		this->bindFrameBuffer(win);

// 		if (STM_LCDShutter == settings.mStereoMethod)
// 		{
// 			mStereoMethod = SM_None;
// 			if ((mDXGISubVersion >= 2) && mGIFactory2->IsWindowedStereoEnabled())
// 			{
// 				mStereoMethod = SM_DXGI;
// 			}
// 			if (SM_None == mStereoMethod)
// 			{
// 				if (win->getAdapter().getDescription().find(L"NVIDIA", 0) != std::wstring::npos)
// 				{
// 					mStereoMethod = SM_NV3DVision;
// 					RenderFactory& rf = Context::getInstance().getRenderFactoryInstance();
// 					uint32_t const w = win->getWidth();
// 					uint32_t const h = win->getHeight();
// 
// 				}
// 			}
// 		}

		if (mD3DFeatureLevel >= D3D_FEATURE_LEVEL_10_0)
		{
			D3D11_QUERY_DESC desc;
			desc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
			desc.MiscFlags = 0;

			ID3D11Query* disjoint_query;
			mD3D11Device->CreateQuery(&desc, &disjoint_query);
			mTimestampDisJointQuery = MakeComPtr(disjoint_query);
		}
	}

	void D3D11RenderEngine::doBindFrameBuffer(FrameBufferPtr const & fb)
	{
		BOOST_ASSERT(mD3D11Device);
		BOOST_ASSERT(fb);

	}

	void D3D11RenderEngine::resetRenderStates()
	{
		//»Ö¸´äÖÈ¾×´Ì¬
	}

	void D3D11RenderEngine::doSuspend()
	{
		if (mD3D11RuntimeSubVer >= 2)
		{
			IDXGIDevice3* dxgi_device = nullptr;
			mD3D11Device->QueryInterface(__uuidof(IDXGIDevice3), reinterpret_cast<void**>(dxgi_device));
			if (dxgi_device != nullptr)
			{
				dxgi_device->Trim();
				dxgi_device->Release();
			}
		}
	}
	void D3D11RenderEngine::doResume()
	{

	}

	HRESULT D3D11RenderEngine::d3d11CreateDevice(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE driver_type, HMODULE Software, UINT flags, D3D_FEATURE_LEVEL const * pFeatureLevels, UINT featureLveleCount, UINT SDKVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) const
	{
		return mDynamicD3D11CreateDevice(pAdapter, driver_type, Software, flags, pFeatureLevels, featureLveleCount, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);
	}
}