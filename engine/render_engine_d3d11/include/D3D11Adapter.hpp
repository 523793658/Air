#ifndef _D3D11Adapter_H_
#define _D3D11Adapter_H_
#pragma once

#include "PreDeclare.h"
#include "D3D11Typedefs.hpp"
#include "D3D11VideoMode.hpp"
namespace Air
{
	class D3D11Adapter
	{
	public:
		D3D11Adapter();
		D3D11Adapter(uint32_t adapter_no, IDXGIAdapter1Ptr const & adapter);

		void enumerate();
		std::wstring const getDescription() const;
		void resetAdapter(IDXGIAdapter1Ptr const &ada);

		uint32_t getAdapterNo() const
		{
			return mAdapterNo;
		}
		IDXGIAdapter1Ptr const & getAdapter() const
		{
			return mAdapter;
		}

		DXGI_FORMAT getDesktopFormat() const
		{
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		size_t getNumVideoMode() const;
		D3D11VideoMode const & getVideoMode(size_t index) const;

	private:
		uint32_t mAdapterNo;
		IDXGIAdapter1Ptr mAdapter;
		DXGI_ADAPTER_DESC1 mAdapterDesc;
		std::vector < D3D11VideoMode > mModes;
	};

	typedef std::shared_ptr<D3D11Adapter> D3D11AdapterPtr;
}



#endif