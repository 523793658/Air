#ifndef _Globals_H_
#define _Globals_H_
#pragma once

#include "PlatformProcess.h"
namespace Air
{


extern bool GIsGameThreadIdInitialized;

extern uint32_t GGameThreadId;
extern uint32_t GRenderThreadId;

extern bool GIsBuildMachine;


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
