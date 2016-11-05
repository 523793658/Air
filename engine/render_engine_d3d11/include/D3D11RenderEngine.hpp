#ifndef Air_D3D11_Render_Engine_H_
#define Air_D3D11_Render_Engine_H_
#pragma once
#include "rendersystem/include/RenderEngine.hpp"

namespace Air
{
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

		void forceFlush();


		bool isFullScreen() const;
		void setFullScreen(bool fs) const;

	};
}




#endif