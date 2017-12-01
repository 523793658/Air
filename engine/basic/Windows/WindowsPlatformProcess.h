#ifndef _WindowsPlatformProcess_H_
#define _WindowsPlatformProcess_H_
#pragma once
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Windows/WindowsSystemInclude.h"


namespace Air
{

	

	struct ProcHandle : public TProcHandle<Windows::HANDLE, nullptr>
	{
	public:
		FORCEINLINE ProcHandle()
			:TProcHandle()
		{}

		FORCEINLINE explicit ProcHandle(HandleType ohter)
			: TProcHandle(ohter)
		{}

	};


	struct WindowsPlatformProcess
		: GenericPlatFormProcess
	{
	public:
		static uint32_t getCurrentProcessId();

		static bool isProcRunning(ProcHandle & handle);

		static void waitForProc(ProcHandle & handle);

		static bool getProcReturnCode(ProcHandle & handle, int32_t * returnCode);

		static void sleep(float time);

		static void closeProc(ProcHandle& handle);

		static void terminateProc(ProcHandle & handle, bool killTree  = false);
	};

	typedef WindowsPlatformProcess PlatformProcess;

}
#endif
