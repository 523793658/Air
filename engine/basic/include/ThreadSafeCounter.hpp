#ifndef _ThreadSafeCounter_H_
#pragma _ThreadSafeCounter_H_
#pragma once
#include "basic/include/Config.h"
#include "basic/include/PlatformAtomics.hpp"
namespace Air
{
	class ThreadSafeCounter
	{
	public:
		typedef int32_t IntegerType;

		ThreadSafeCounter()
		{
			mCounter = 0;
		}

		ThreadSafeCounter(const ThreadSafeCounter& other)
		{
			mCounter = other.getValue();
		}

		ThreadSafeCounter(int32_t value)
			:mCounter(value)
		{

		}

		int32_t increment()
		{
			return FPlatformAtomics::interLockedIncrement(&mCounter);
		}

		int32_t add(int32_t amount)
		{
			return FPlatformAtomics::interLockedAdd(&mCounter, amount);
		}

		int32_t decrement()
		{
			return FPlatformAtomics::interLockedDecrement(&mCounter);
		}

		int32_t subtract(int32_t amount)
		{
			return FPlatformAtomics::interLockedAdd(&mCounter, -amount);
		}

		int32_t set(int32_t value)
		{
			return FPlatformAtomics::interLockedExchange(&mCounter, value);
		}

		int32_t reset()
		{
			return FPlatformAtomics::interLockedExchange(&mCounter, 0);
		}

		int32_t getValue() const
		{
			return mCounter;
		}
	private:
		void operator = (const ThreadSafeCounter& other) {}

		volatile int32_t mCounter;
	};
}
#endif
