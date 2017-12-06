#ifndef _WindowsPlatformProcess_H_
#define _WindowsPlatformProcess_H_
#pragma once
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Windows/WindowsSystemInclude.h"
#include "basic/include/STLVector.hpp"


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

		static ProcHandle createProc(std::string const & url, std::string const & params, bool launchDetached, bool launchHidden, bool launchReallyHidden, uint32_t* outProcessId, int32_t priorityModifier, std::string const & optionalWorkingDirecoty, void* pipeWriteChild, void * pipeReadChild = nullptr);

		static void addDllDirectory(std::string directory);

	private:
		static std::vector<std::string> mDllDirectoryStack;
		static std::vector<std::string> mDllDirectoies;
	};

	typedef WindowsPlatformProcess PlatformProcess;

}
#endif
