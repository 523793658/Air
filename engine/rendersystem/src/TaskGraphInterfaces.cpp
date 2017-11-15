#include "Context.h"
#include "rendersystem/include/TaskGraphInterfaces.hpp"

#if AIR_PLATFORM_XBOXONE || AIR_PLATFORM_PS
#define CREATE_HIPRI_TASK_THREADS (1)
#define CREATE_BACKGROUND_TASK_THREADS (1)
#else
#define CREATE_HIPRI_TASK_THREADS (0)
#define CREATE_BACKGROUND_TASK_THREADS (0)
#endif


namespace ENamedThreads
{
	Type RenderThread = ENamedThreads::GameThread;
	Type RenderThread_Local = ENamedThreads::GameThread_Local;
	int32_t bHashBackgroundThreads = CREATE_BACKGROUND_TASK_THREADS;
	int32_t bHashHighPriorityThreads = CREATE_HIPRI_TASK_THREADS;

}