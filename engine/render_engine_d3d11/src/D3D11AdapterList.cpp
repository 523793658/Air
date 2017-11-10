#include "Context.h"
#include "basic/include/Util.h"
#include "basic/include/ErrorHanding.hpp"
#include "basic/include/COMPtr.hpp"
#include "D3D11AdapterList.hpp"

namespace Air
{


	D3D11AdapterList::D3D11AdapterList()
		:mCurrentAdapter(0)
	{
	}

	void D3D11AdapterList::destroy()
	{
		mAdapters.clear();
		mCurrentAdapter = 0;
	}

	void D3D11AdapterList::enumerate(IDXGIFactory1Ptr const & gi_factory)
	{
		UINT adapter_no = 0;
		IDXGIAdapter1* dxgi_adapter = nullptr;
		while (gi_factory->EnumAdapters1(adapter_no, &dxgi_adapter) != DXGI_ERROR_NOT_FOUND)
		{
			if (dxgi_adapter != nullptr)
			{
				D3D11AdapterPtr adapter = MakeSharedPtr<D3D11Adapter>(adapter_no, MakeComPtr(dxgi_adapter));
				adapter->enumerate();
				mAdapters.push_back(adapter);
			}
			++adapter_no;
		}
		if (mAdapters.empty())
		{
			TERRC(std::errc::function_not_supported);
		}
	}


	size_t D3D11AdapterList::numAdapter()const
	{
		return mAdapters.size();
	}

	D3D11AdapterPtr const & D3D11AdapterList::getAdapter(size_t index) const
	{
		return mAdapters[index];
	}

	uint32_t D3D11AdapterList::getCurrentAdapterIndex() const
	{
		return mCurrentAdapter;
	}

	void D3D11AdapterList::setCurrentAdapterIndex(uint32_t index)
	{
		mCurrentAdapter = index;
	}

}
