#ifndef _Globals_H_
#define _Globals_H_
#pragma once

#include "PlatformProcess.h"
namespace Air
{


extern bool GIsGameThreadIdInitialized;

uint32_t GGameThreadId = 0;
uint32_t GRenderThreadId = 0;


FORCEINLINE bool isInGameThread()
{
	if (GIsGameThreadIdInitialized)
	{
		const uint32_t currentThreadId = PlatformProcess::getCurrentProcessId();
		return currentThreadId == GGameThreadId;
	}
	return true;
}
}


#endif
