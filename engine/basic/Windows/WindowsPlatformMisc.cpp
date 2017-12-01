#include "basic/include/Basic.h"
#include "Windows/WindowsPlatformMisc.h"
#include "Windows/WindowsPlatformProcess.h"
#include "Windows/WindowsHWrapper.h"
#include "basic/include/ErrorHanding.hpp"
#include <time.h>
#include <mmsyscom.h>
#include <rpcsal.h>
#include <gameux.h>
#include <ShlObj.h>
#include <IntShCut.h>
#include <shellapi.h>
#include <IPHlpApi.h>
#include <VersionHelpers.h>
namespace Air
{
	int32_t WindowsPlatformMisc::getNumberOfCoresIncludingHyperThreads()
	{
		static int32_t coreCount = 0;
		if (coreCount == 0)
		{
			SYSTEM_INFO SI;
			GetSystemInfo(&SI);
			coreCount = (int32_t)SI.dwNumberOfProcessors;
		}
		return coreCount;
	}
	
	bool WindowsPlatformMisc::isDebuggerPresent()
	{
		return !!::IsDebuggerPresent();
	}

	void WindowsPlatformMisc::createGuid(struct Guid& result)
	{
		Verify(CoCreateGuid((GUID*)&result) == S_OK);
	}
}