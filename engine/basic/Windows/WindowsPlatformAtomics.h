#ifndef _WindowsPlatformAtomics_H_
#define _WindowsPlatformAtomics_H_
#pragma once
#include "HAL/CoreTypes.h"
#include "GenericPlatform/GenericPlatformAtomics.h"
struct WindowsPlatformAtomics
	: public GenericPlatformAtomics
{
	static FORCEINLINE int32_t InterlockedIncrement(volatile int32_t* Value)
	{
		return (int32_t)_InterlockedIncrement((int32_t*)Value);
	}

#if PLATFORM_HAS_64BIT_ATOMICS
	static FORCEINLINE int64_t InterlockedIncrement(volatile int64_t* Value)
	{
#if PLATFORM_64BITS
		return (int64_t)::_InterlockedIncrement64((int64_t*)Value);
#else
		// No explicit instruction for 64-bit atomic increment on 32-bit processors; has to be implemented in terms of CMPXCHG8B
		for (;;)
		{
			int64_t OldValue = *Value;
			if (_InterlockedCompareExchange64(Value, OldValue + 1, OldValue) == OldValue)
			{
				return OldValue + 1;
			}
		}
#endif
	}
#endif

	static FORCEINLINE int32_t InterlockedDecrement(volatile int32_t* Value)
	{
		return (int32_t)::_InterlockedDecrement((long*)Value);
	}

#if PLATFORM_HAS_64BIT_ATOMICS
	static FORCEINLINE int64_t InterlockedDecrement(volatile int64_t* Value)
	{
#if PLATFORM_64BITS
		return (int64_t)::_InterlockedDecrement64((int64_t*)Value);
#else
		// No explicit instruction for 64-bit atomic decrement on 32-bit processors; has to be implemented in terms of CMPXCHG8B
		for (;;)
		{
			int64_t OldValue = *Value;
			if (_InterlockedCompareExchange64(Value, OldValue - 1, OldValue) == OldValue)
			{
				return OldValue - 1;
			}
		}
#endif
	}
#endif

	static FORCEINLINE int32_t InterlockedAdd(volatile int32_t* Value, int32_t Amount)
	{
		return (int32_t)::_InterlockedExchangeAdd((long*)Value, (long)Amount);
	}

#if PLATFORM_HAS_64BIT_ATOMICS
	static FORCEINLINE int64_t InterlockedAdd(volatile int64_t* Value, int64_t Amount)
	{
#if PLATFORM_64BITS
		return (int64_t)::_InterlockedExchangeAdd64((int64_t*)Value, (int64_t)Amount);
#else
		// No explicit instruction for 64-bit atomic add on 32-bit processors; has to be implemented in terms of CMPXCHG8B
		for (;;)
		{
			int64_t OldValue = *Value;
			if (_InterlockedCompareExchange64(Value, OldValue + Amount, OldValue) == OldValue)
			{
				return OldValue + Amount;
			}
		}
#endif
	}
#endif

	static FORCEINLINE int32_t InterlockedExchange(volatile int32_t* Value, int32_t Exchange)
	{
		return (int32_t)::_InterlockedExchange((long*)Value, (long)Exchange);
	}

#if PLATFORM_HAS_64BIT_ATOMICS
	static FORCEINLINE int64_t InterlockedExchange(volatile int64_t* Value, int64_t Exchange)
	{
#if PLATFORM_64BITS
		return ::_InterlockedExchange64(Value, Exchange);
#else
		// No explicit instruction for 64-bit atomic exchange on 32-bit processors; has to be implemented in terms of CMPXCHG8B
		for (;;)
		{
			int64_t OldValue = *Value;
			if (_InterlockedCompareExchange64(Value, Exchange, OldValue) == OldValue)
			{
				return OldValue;
			}
		}
#endif
	}
#endif

	static FORCEINLINE void* InterlockedExchangePtr(void** Dest, void* Exchange)
	{

	}

	static FORCEINLINE int32_t InterlockedCompareExchange(volatile int32_t* Dest, int32_t Exchange, int32_t Comparand)
	{
		return (int32_t)::_InterlockedCompareExchange((long*)Dest, (long)Exchange, (long)Comparand);
	}

#if PLATFORM_HAS_64BIT_ATOMICS
	static FORCEINLINE int64_t InterlockedCompareExchange(volatile int64_t* Dest, int64_t Exchange, int64_t Comparand)
	{

	}
#endif



protected:
	
};


typedef WindowsPlatformAtomics PlatformAtomics;
#endif
