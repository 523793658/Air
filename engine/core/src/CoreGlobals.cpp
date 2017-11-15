#include "Context.h"
#include "CoreGlobals.hpp"

namespace Air
{
	AIR_CORE_API joiner<void> * GRenderingThread = nullptr;

	AIR_CORE_API int32_t GIsRenderingThreadSuspended = 0;

	bool isInRenderingThread()
	{
		return !GRenderingThread || GIsRenderingThreadSuspended || (threadof() == GRenderingThread->get_thread_id());
	}

}