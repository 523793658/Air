#ifndef _GenericPlatformProcess_H_
#define _GenericPlatformProcess_H_
#pragma once
#include "HAL/CoreTypes.h"
namespace Air
{

	template<typename T, T InvalidHandleValue>
	struct TProcHandle
	{
		typedef T HandleType;
	public:
		FORCEINLINE TProcHandle()
			: mHandle(InvalidHandleValue)
		{}

		FORCEINLINE explicit TProcHandle(T other)
			:mHandle(other)
		{}

		FORCEINLINE TProcHandle& operator = (const TProcHandle & other)
		{
			if (this != &other)
			{
				mHandle = other.mHandle;
			}
			return *this;
		}

		FORCEINLINE T get() const
		{
			return mHandle;
		}
		FORCEINLINE void reset()
		{
			mHandle = InvalidHandleValue;
		}

		FORCEINLINE bool isValid() const
		{
			return mHandle != InvalidHandleValue;
		}

		FORCEINLINE bool close()
		{
			return isValid();
		}




	protected:
		T mHandle;
	};


	struct ProcHandle;

	struct GenericPlatFormProcess
	{
		static bool supportsMultithreading();

		static bool isProcRunning(ProcHandle & handle);

		static void waitForProc(ProcHandle & handle);

		static bool getProcReturnCode(ProcHandle & handle, int32_t * returnCode);

		static void sleep(float time);

		static void closeProc(ProcHandle& handle);

		static void terminateProc(ProcHandle & handle, bool killTree = false);

		static const std::string_view shaderDir();
		static void setShaderDir(const std::string_view path);
	};
}

#endif
