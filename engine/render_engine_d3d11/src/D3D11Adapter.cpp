#include <array>

#include "Engine.h"
#include "render_engine_d3d11/include/D3D11VideoMode.hpp"
#include "render_engine_d3d11/include/D3D11Adapter.hpp"

namespace Air
{

	D3D11Adapter::D3D11Adapter()
		:mAdapterNo(0)
	{

	}
	D3D11Adapter::D3D11Adapter(uint32_t adapterNo, IDXGIAdapter1Ptr const & adapter)
		: mAdapterNo(adapterNo)
	{
		this->resetAdapter(adapter);
	}



	void D3D11Adapter::enumerate()
	{
		std::array<DXGI_FORMAT, 5> formats;
		formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		formats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		formats[2] = DXGI_FORMAT_B8G8R8A8_UNORM;
		formats[3] = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		formats[4] = DXGI_FORMAT_R10G10B10A2_UNORM;

		UINT i = 0;
		IDXGIOutput * output = nullptr;
		while (mAdapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
		{
			if (output != nullptr)
			{
				for (auto const & format : formats)
				{
					UINT num = 0;
					output->GetDisplayModeList(format, DXGI_ENUM_MODES_SCALING, &num, 0);
					if (num > 0)
					{
						std::vector<DXGI_MODE_DESC> mode_descs(num);
						output->GetDisplayModeList(format, DXGI_ENUM_MODES_SCALING, &num, &mode_descs[0]);
						for (auto const & mode_desc : mode_descs)
						{
							D3D11VideoMode const video_mode(mode_desc.Width, mode_desc.Height, mode_desc.Format);
							if (std::find(mModes.begin(), mModes.end(), video_mode) == mModes.end())
							{
								mModes.push_back(video_mode);
							}
						}
					}
				}
				output->Release();
				output = nullptr;
			}
			++i;
		}
		std::sort(mModes.begin(), mModes.end());
	}

	void D3D11Adapter::resetAdapter(IDXGIAdapter1Ptr const &ada)
	{
		mAdapter = ada;
		mAdapter->GetDesc1(&mAdapterDesc);
		mModes.resize(0);

		IDXGIAdapter2* adapter2;
		mAdapter->QueryInterface(__uuidof(IDXGIAdapter2), reinterpret_cast<void**>(&adapter2));
		if (adapter2 != nullptr)
		{
			DXGI_ADAPTER_DESC2 desc2;
			adapter2->GetDesc2(&desc2);
			memcpy(mAdapterDesc.Description, desc2.Description, sizeof(desc2.Description));
			mAdapterDesc.VendorId = desc2.VendorId;
			mAdapterDesc.DeviceId = desc2.DeviceId;
			mAdapterDesc.SubSysId = desc2.SubSysId;
			mAdapterDesc.Revision = desc2.Revision;
			mAdapterDesc.DedicatedVideoMemory = desc2.DedicatedVideoMemory;
			mAdapterDesc.DedicatedSystemMemory = desc2.DedicatedSystemMemory;
			mAdapterDesc.SharedSystemMemory = desc2.SharedSystemMemory;
			mAdapterDesc.AdapterLuid = desc2.AdapterLuid;
			mAdapterDesc.Flags = desc2.Flags;
			adapter2->Release();
		}
	}

	std::wstring const D3D11Adapter::getDescription() const
	{
		return std::wstring(mAdapterDesc.Description);
	}

	size_t D3D11Adapter::getNumVideoMode() const
	{
		return mModes.size();
	}
	D3D11VideoMode const & D3D11Adapter::getVideoMode(size_t index) const
	{
		return mModes[index];
	}
}