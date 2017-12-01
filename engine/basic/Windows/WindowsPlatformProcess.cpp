#include "Windows/WindowsPlatformProcess.h"

#include "Windows/WindowsHWrapper.h"
#include <shellapi.h>
#include <ShlObj.h>
#include <LM.h>
#include <Psapi.h>
#include <TlHelp32.h>
namespace Air
{

	namespace ProcessConstants
	{
		uint32_t WIN_STD_INPUT_HANDLE = STD_INPUT_HANDLE;
		uint32_t WIN_STD_OUTPUT_HANDLE = STD_OUTPUT_HANDLE;
		uint32_t WIN_ATTACH_PARENT_PROCESS = ATTACH_PARENT_PROCESS;
		uint32_t WIN_STILL_ACTIVE = STILL_ACTIVE;
	}


	uint32_t WindowsPlatformProcess::getCurrentProcessId()
	{
		return ::GetCurrentProcessId();
	}

	bool WindowsPlatformProcess::isProcRunning(ProcHandle & handle)
	{
		bool bApplicationRunning = true;
		uint32_t waitResult = ::WaitForSingleObject(handle.get(), 0);
		if (waitResult != WAIT_TIMEOUT)
		{
			bApplicationRunning = false;
		}
		return bApplicationRunning;
	}

	void WindowsPlatformProcess::waitForProc(ProcHandle & handle)
	{
		::WaitForSingleObject(handle.get(), INFINITE);
	}

	bool WindowsPlatformProcess::getProcReturnCode(ProcHandle & handle, int32_t * returnCode)
	{
		return ::GetExitCodeProcess(handle.get(), (::DWORD*)returnCode) && *((uint32_t*)returnCode) != ProcessConstants::WIN_STILL_ACTIVE;
	}

	void WindowsPlatformProcess::sleep(float time)
	{
		uint32_t Milliseconds = (uint32_t)(time * 1000.0);
		if (Milliseconds == 0)
		{
			::SwitchToThread();
		}
		::Sleep(Milliseconds);
	}

	void WindowsPlatformProcess::closeProc(ProcHandle& handle)
	{
		if (handle.isValid())
		{
			::CloseHandle(handle.get());
			handle.reset();
		}
	}
	void WindowsPlatformProcess::terminateProc(ProcHandle & handle, bool killTree)
	{
		if (killTree)
		{
			HANDLE snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (snapshot != INVALID_HANDLE_VALUE)
			{
				::DWORD processId = ::GetProcessId(handle.get());
				PROCESSENTRY32 entry;
				entry.dwSize = sizeof(PROCESSENTRY32);
				if (::Process32First(snapshot, &entry))
				{
					do 
					{
						if (entry.th32ParentProcessID == processId)
						{
							HANDLE childProcHandle = ::OpenProcess(PROCESS_ALL_ACCESS, 0, entry.th32ProcessID);
							if (childProcHandle)
							{
								ProcHandle childHandle(childProcHandle);
								terminateProc(childHandle, killTree);
							}
						}
					} while (::Process32Next(snapshot, &entry));
				}
			}
			TerminateProcess(handle.get(), 0);
		}
	}
}