#ifndef _D3D11AdapterList_H_
#define _D3D11AdapterList_H_
#pragma once

#include "PreDeclare.h"
#include "D3D11Adapter.hpp"

namespace Air
{
	class D3D11AdapterList
	{
	public:
		D3D11AdapterList();

		void destroy();

		void enumerate(IDXGIFactory1Ptr const & gi_factory);

		size_t numAdapter()const;

		D3D11AdapterPtr const & getAdapter(size_t index) const;

		uint32_t getCurrentAdapterIndex() const;

		void setCurrentAdapterIndex(uint32_t index);

	private:
		std::vector<D3D11AdapterPtr>	mAdapters;
		uint32_t						mCurrentAdapter;


	};
}




#endif