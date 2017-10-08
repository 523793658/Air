#ifndef Air_D3D11_Render_Engine_H_
#define Air_D3D11_Render_Engine_H_
#pragma once

#include <set>
#include <map>
#include "../../external/dxsdk/Include/dxgi1_5.h"

#include "render_engine_d3d11/include/D3D11AdapterList.hpp"

#include "render_engine_d3d11/include/D3D11Typedefs.hpp"

#include "rendersystem/include/ShaderObject.hpp"

#include "rendersystem/include/RenderEngine.hpp"

namespace Air
{
	struct D3D11RenderEngineCache 
	{
		ID3D11RasterizerState* mRasterizerState{ nullptr };
		ID3D11DepthStencilState* mDepthStencilState{ nullptr };
		uint16_t mStencilRef{ 1 };
		ID3D11BlendState* mBlendState{ nullptr };
		Color mBlendFactor;
		uint32_t mSampleMask;
		ID3D11VertexShader* mVertexShader{ nullptr };
		ID3D11PixelShader* mPixelShader{ nullptr };
		ID3D11GeometryShader* mGeometryShader{ nullptr };
		ID3D11ComputeShader* mComputeShader{ nullptr };
		ID3D11HullShader* mHullShader{ nullptr };
		ID3D11DomainShader* mDomainShader{ nullptr };
		RenderLayout::TopologyType mTopologyType{ RenderLayout::TT_UNKNOWN };
		ID3D11InputLayout* mInputLayout{ nullptr };
		D3D11_VIEWPORT mViewport;
		uint32_t mNumSoBuffs;
		std::vector<ID3D11Buffer*> mVB;
		std::vector<UINT> mVBStride;
		std::vector<UINT> mVBOffset;
		ID3D11Buffer* mIB;

		std::array<std::vector<std::tuple<void*, uint32_t, uint32_t>>, ShaderObject::ST_NumShaderTypes> mShaderSrvSrc;
		std::array<std::vector<ID3D11ShaderResourceView*>, ShaderObject::ST_NumShaderTypes> mShaderSRVPtr;
		std::array<std::vector<ID3D11SamplerState*>, ShaderObject::ST_NumShaderTypes> mShaderSamplerPrt;
		std::array<std::vector<ID3D11Buffer*>, ShaderObject::ST_NumShaderTypes> mShaderCBPtr;
		std::vector<ID3D11UnorderedAccessView*> mRenderUAVPrt;
		std::vector<uint32_t> mRenderUavInitCount;
		std::vector<ID3D11UnorderedAccessView*> mComputeUavPtr;
		std::vector<uint32_t> mComputeUavInitCount;
		std::vector<ID3D11RenderTargetView*> mRTVPtr;
		ID3D11DepthStencilView* mDSVPtr;
	};

	class D3D11RenderEngine : public RenderEngine
	{
	public:
		D3D11RenderEngine();
		~D3D11RenderEngine();
		std::wstring const & getName() const;

		bool requiresFlipping() const
		{
			return true;
		}

		void beginFrame() override;
		void endFrame() override;

		void invalidRTVCache();

		void updateGPUTimestampsFrequency() override;

		IDXGIFactory1* getDXGIFactory1() const;
		IDXGIFactory2* getDXGIFactory2() const;
		IDXGIFactory3* getDXGIFactory3() const;
		IDXGIFactory4* getDXGIFactory4() const;
		IDXGIFactory5* getDXGIFactory5() const;

		uint8_t getDXGISubVer() const;

		ID3D11Device* getD3DDevice() const;
		ID3D11Device1* getD3DDevice1() const;
		ID3D11Device2* getD3DDevice2() const;
		ID3D11Device3* getD3DDevice3() const;
		
		ID3D11DeviceContext* getD3DDeviceContext() const;
		ID3D11DeviceContext1* getD3DDeviceContext1() const;
		ID3D11DeviceContext2* getD3DDeviceContext2() const;
		ID3D11DeviceContext3* getD3DDeviceContext3() const;

		uint8_t getD3D11RuntimeSubVer() const;

		D3D_FEATURE_LEVEL getDeviceFeatureLevel() const;

		void setD3DDevice(ID3D11Device* device, ID3D11DeviceContext* context, D3D_FEATURE_LEVEL feature_level);

		void detectD3D11Runtime(ID3D11Device* device, ID3D11DeviceContext* imm_ctx);

		void fillRenderDeviceCaps();


		void forceFlush();

		void destroy();


		HRESULT d3d11CreateDevice(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE driver_type, HMODULE Software, UINT flags, D3D_FEATURE_LEVEL const * pFeatureLevels, UINT featureLveleCount, UINT SDKVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) const;

		bool isFullScreen() const;
		void setFullScreen(bool fs);


		void setVertexShader(ID3D11VertexShader* shader);
		void setPixelShader(ID3D11PixelShader* shader);
		void setGeometryShader(ID3D11GeometryShader* shader);
		void setComputeShader(ID3D11ComputeShader* shader);
		void setHullShader(ID3D11HullShader* shader);
		void setDomainShader(ID3D11DomainShader* shader);
		void setRenderTargets(UINT num_rtvs, ID3D11RenderTargetView* const * rtvs, ID3D11DepthStencilView* dsv);
		void setViewports(UINT num_viewports, D3D11_VIEWPORT const * pViewports);

		void setShaderResources(ShaderObject::ShaderType st, std::vector<std::tuple<void*, uint32_t, uint32_t>> const & srv_srcs, std::vector<ID3D11ShaderResourceView*> const & srvs);

		void setSamplers(ShaderObject::ShaderType st, std::vector<ID3D11SamplerState*> const & samplers);



		void setConstantBuffers(ShaderObject::ShaderType st, std::vector<ID3D11Buffer*> const & cbs);

		void detachSRV(void* rtv_src, uint32_t rt_frist_subres, uint32_t rt_num_subres);

		void csSetUnorderedAccessViews(UINT start_slot, UINT num_uavs, ID3D11UnorderedAccessView* const * uavs, UINT const * uav_init_counts);

		void setRasterizerState(ID3D11RasterizerState* ras);
		void setDepthStencilState(ID3D11DepthStencilState* dss, uint16_t stencil_ref);
		void setBlendState(ID3D11BlendState* bs, Color const & blend_factor, uint32_t sample_mask);

		char const * getVertexShaderProfile() const
		{
			return mVSProfile;
		}
		char const * getPixelShaderProfile() const
		{
			return mPSProfile;
		}
		char const * getGeometryShaderProfile() const
		{
			return mGSProfile;
		}
		char const * getComputeShaderProfile() const
		{
			return mCSProfile;
		}
		char const * getHullShaderProfile() const
		{
			return mHSProfile;
		}
		char const * getDomainShaderProfile() const
		{
			return mDSProfile;
		}
	private:
		bool isVertexFormatSupport(ElementFormat elem_fmt);
		bool isTextureFormatSupport(ElementFormat elem_fmt);
		bool isRenderTargetFormatSupport(ElementFormat elem_fmt, uint32_t sample_count, uint32_t sample_quality);
		virtual void doCreateRenderWindow(std::string const & name, RenderSettings const & settings);
		void doBindFrameBuffer(FrameBufferPtr const & fb);
		D3D11AdapterPtr const & getActiveAdapter() const;
		void resetRenderStates();
		virtual void doRender(RenderEffect const &effect, RenderTechnique const & tech, RenderLayout const & rl) override;
		void doSuspend();
		void doResume();

	private:
		typedef HRESULT(WINAPI *CreateDXGIFactory1Func)(REFIID riid, void** ppFactory);
		typedef HRESULT(WINAPI *D3D11CreateDeviceFunc)(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, D3D_FEATURE_LEVEL const * pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext);

		CreateDXGIFactory1Func mDynamicCreateDXGIFactory1;
		D3D11CreateDeviceFunc mDynamicD3D11CreateDevice;

		IDXGIFactory1Ptr mGIFactory1;
		IDXGIFactory2Ptr mGIFactory2;
		IDXGIFactory3Ptr mGIFactory3;
		IDXGIFactory4Ptr mGIFactory4;
		IDXGIFactory5Ptr mGIFactory5;
		uint8_t mDXGISubVersion;

		ID3D11DeviceContextPtr mD3DIMMContext;
		ID3D11DeviceContext1Ptr mD3DIMMContext1;
		ID3D11DeviceContext2Ptr mD3DIMMContext2;
		ID3D11DeviceContext3Ptr mD3DIMMContext3;


		ID3D11DevicePtr mD3D11Device;
		ID3D11Device1Ptr mD3D11Device1;
		ID3D11Device2Ptr mD3D11Device2;
		ID3D11Device3Ptr mD3D11Device3;

		uint8_t mD3D11RuntimeSubVer;


		D3D_FEATURE_LEVEL mD3DFeatureLevel;

		D3D11AdapterList mAdapterList;

		std::set<ElementFormat> mVertexFormats;
		std::set<ElementFormat> mTextureFormats;
		std::map<ElementFormat, std::vector<std::pair<uint32_t, uint32_t>>> mRendertargetFormats;


	private:
		HMODULE mModDXGI;
		HMODULE mModD3D11;


		uint32_t mNativeShaderVersion;
		uint32_t mNativeShaderFourcc;

		ID3D11QueryPtr mTimestampDisJointQuery;

		enum StereoMethod
		{
			SM_None,
			SM_DXGI,
			SM_NV3DVision,
			SM_AMDQuadBuffer
		};
		
		StereoMethod mStereoMethod;
		double mInvTimestampFreq;

		D3D11RenderEngineCache mRenderCache;


		char const * mVSProfile;
		char const * mPSProfile;
		char const * mGSProfile;
		char const * mCSProfile;
		char const * mHSProfile;
		char const * mDSProfile;

	};
}




#endif