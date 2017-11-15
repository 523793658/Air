#ifndef _WindowsPlatformAtomics_H_
#define _WindowsPlatformAtomics_H_
#pragma once
#include "basic/include/GenericPlatformAtomics.hpp"
#include "basic/windows/WindowsSystemIncludes.h"
struct WindowsPlatformAtomics : public GenericPlatformAtomics
{
	static FORCEINLINE int32_t interLockedIncrement(volatile int32_t* value)
	{
		return (int32_t)_InterlockedIncrement((long*)value);
	}

	static FORCEINLINE int32_t interLockedAdd(volatile int32_t * value, int32_t amount)
	{
		return (int32_t)::_InterlockedExchangeAdd((long*)value, (long)amount);
	}

#if defined(AIR_CPU_X64)|| defined(AIR_CPU_ARM64)
	static FORCEINLINE int64_t interLockedAdd(volatile int64_t * value, int64_t amount)
	{
		return (int64_t)::_InterlockedExchangeAdd64((int64_t*)value, (int64_t)amount);
	}
#endif


	static FORCEINLINE int32_t interLockedDecrement(volatile int32_t * value)
	{
		return (int32_t)::_InterlockedDecrement((long*)value);
	}

#if defined(AIR_CPU_X64)|| defined(AIR_CPU_ARM64)
	static FORCEINLINE int64_t InterlockedDecrement(volatile int64_t* Value)
	{
		return (int64_t)::_InterlockedDecrement64((int64_t*)Value);
	}
#endif

	static FORCEINLINE int32_t interLockedExchange(volatile int32_t* vlaue, int32_t exchange)
	{
		return (int32_t)::_InterlockedExchange((long*)vlaue, (long)exchange);
	}


#if defined(AIR_CPU_X64)|| defined(AIR_CPU_ARM64)
	static FORCEINLINE int64_t interLockedExchange(volatile int64_t* Value, int64_t exchange)
	{
		return (int64_t)::_InterlockedExchange64((int64_t*)Value, (int64_t)exchange);
	}
#endif


};

typedef WindowsPlatformAtomics FPlatformAtomics;


#endif
