#include "Windows/WindowsPlatformProcess.h"

#include "Windows/WindowsHWrapper.h"
#include <shellapi.h>
#include <ShlObj.h>
#include <LM.h>
#include <Psapi.h>
#include <TlHelp32.h>
namespace Air
{

	std::vector<std::string> WindowsPlatformProcess::mDllDirectoryStack;

	std::vector<std::string> WindowsPlatformProcess::mDllDirectoies;
		
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
	ProcHandle WindowsPlatformProcess::createProc(std::string const & url, std::string const & params, bool launchDetached, bool launchHidden, bool launchReallyHidden, uint32_t* outProcessId, int32_t priorityModifier, std::string const & optionalWorkingDirecoty, void* pipeWriteChild, void * pipeReadChild /* = nullptr */)
	{
		SECURITY_ATTRIBUTES attr;
		attr.nLength = sizeof(SECURITY_ATTRIBUTES);
		attr.lpSecurityDescriptor = NULL;
		attr.bInheritHandle = true;
		uint32_t createFlags = NORMAL_PRIORITY_CLASS;
		if (priorityModifier < 0)
		{
			createFlags = (priorityModifier == -1) ? BELOW_NORMAL_PRIORITY_CLASS : IDLE_PRIORITY_CLASS;
		}
		else if(priorityModifier > 0)
		{
			createFlags = (priorityModifier == 1)
				? ABOVE_NORMAL_PRIORITY_CLASS : HIGH_PRIORITY_CLASS;
		}
		if (launchDetached)
		{
			createFlags |= DETACHED_PROCESS;
		}
		uint32_t dwFlags = 0;
		uint16_t showWindowFlags = SW_HIDE;
		if (launchReallyHidden)
		{
			dwFlags = STARTF_USESHOWWINDOW;
		}
		else if(launchHidden)
		{
			dwFlags = STARTF_USESHOWWINDOW;
			showWindowFlags = SW_SHOWMINNOACTIVE;
		}
		if (pipeWriteChild != nullptr || pipeReadChild != nullptr)
		{
			dwFlags |= STARTF_USESTDHANDLES;
		}
		STARTUPINFOA startupInfo = {
			sizeof(STARTUPINFOA),
			NULL, NULL, NULL,
			(::DWORD)CW_USEDEFAULT,
			(::DWORD)CW_USEDEFAULT,
			(::DWORD)CW_USEDEFAULT,
			(::DWORD)CW_USEDEFAULT,
			(::DWORD)0, (::DWORD)0, (::DWORD)0,
			(::DWORD)dwFlags,
			showWindowFlags,
			0, NULL,
			HANDLE(pipeReadChild),
			HANDLE(pipeWriteChild),
			HANDLE(pipeWriteChild)
		};
		std::string commandLine = "\"" + url + "\" " + params;
		char* commandLineChar = (char*)((commandLine.size() + 1) * sizeof(char));
		memcpy(commandLineChar, commandLine.c_str(), commandLine.size() * sizeof(char));
		PROCESS_INFORMATION procInfo;

		if (!CreateProcessA(NULL, commandLineChar, &attr, &attr, true, (::DWORD)createFlags, NULL, optionalWorkingDirecoty.c_str(), &startupInfo, &procInfo))
		{
			if (outProcessId != nullptr)
			{
				*outProcessId = 0;
			}
			return ProcHandle();
		}
		if (outProcessId != nullptr)
		{
			*outProcessId = procInfo.dwProcessId;
		}
		::CloseHandle(procInfo.hThread);
		return ProcHandle(procInfo.hProcess);
	}

	void WindowsPlatformProcess::addDllDirectory(std::string directory)
	{
		mDllDirectoies.push_back(directory);
	}
}