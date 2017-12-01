#ifndef _WindowsPlatformMisc_H_
#define _WindowsPlatformMisc_H_
#pragma once
#include "HAL/CoreTypes.h"
#include "GenericPlatform/GenericPlatformMisc.h"
namespace Air
{
	struct WindowsPlatformMisc
		: public GenericPlatformMisc
	{
		static int32_t getNumberOfCoresIncludingHyperThreads();

		static bool isDebuggerPresent();

		static void createGuid(struct Guid& result);
	};


	typedef WindowsPlatformMisc PlatformMisc;


}
#endif
