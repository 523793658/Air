#ifndef _Basic_ComPtr_H_
#define _Basic_ComPtr_H_

#pragma once

#include <functional>
#include <iostream>

namespace Air
{
	template<typename T>
	inline std::shared_ptr<T>
		MakeComPtr(T* p)
	{
		return p ? std::shared_ptr<T>(p, std::mem_fn(&T::Release)) : std::shared_ptr<T>();
	}
}


#endif