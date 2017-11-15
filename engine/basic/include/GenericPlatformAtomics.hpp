#ifndef _GenericPlatformAtomics_H_
#define _GenericPlatformAtomics_H_
#pragma once 

struct AIR_ALIGN(16) Int128
{
	int64_t low;
	int64_t high;
}

struct GenericPlatformAtomics
{
#if 0

#endif
public:
	static FORCEINLINE bool callUseCompareExchange128()
	{
		return false;
	}
protected:
	static inline bool isAligned(const volatile void* ptr, const uint32_t alignment = sizeof(void*))
	{
		return !(intptr_t(ptr)&(alignment - 1));
	}
};





#endif
