#ifndef _AlignmentTemplates_H_
#define _AlignmentTemplates_H_
#pragma once
#include "basic/include/Config.h"
#include <stdint.h>

namespace Air
{
	template<typename T>
	inline constexpr T align(const T ptr, int32_t alignment)
	{
		return (T)(((intptr_t)ptr + alignment - 1) & ~(alignment - 1));
	}

	template<typename T>
	inline constexpr T alignDown(const T ptr, int32_t alignment)
	{
		return (T)(((intptr_t)ptr) & ~(alignment - 1));
	}

	static bool isAligned(const volatile void* ptr, const uint32_t alignment)
	{
		return !(uintptr_t(ptr) & (alignment - 1));
	}

	template<class T> inline T alignArbitrary(const T ptr, uint32_t alignment)
	{
		return (T)((((uintptr_t)ptr + alignment - 1) / alignment) * alignment);
	}

}
#endif
