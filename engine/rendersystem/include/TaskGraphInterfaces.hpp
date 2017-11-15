#ifndef _TaskGraphInterface_H_
#define _TaskGraphInterface_H_
#pragma once


namespace ENamedThreads
{
	enum Type
	{
		UnusedAnchor = -1,
#if STATS
		StatsThread,
#endif
		RHIThread,
		AudioThread,
		GameThread,

		ActualRenderingThread = GameThread + 1,
		AnyThread = 0xff,

		MainQueue = 0x000,
		LocalQueue = 0x100,

		NumQueues	= 2,
		ThreadIndexMask = 0xff,
		QueueIndexMask = 0x100,
		QueueIndexShift  = 8,

		NormalTaskPriority = 0x000,
		HighTaskPriority = 0x200,
		NumTaskPriorities = 2,

		TaskPriorityMask = 0x200,
		TaskPriorityShift = 9,
		NormalThreadPriority = 0x000,
		HighThreadPriority = 0x400,
		BackgroundThreadPriority = 0x800,
		NumThreadPriorities = 3,
		ThreadPriorityMask = 0xC00,
		ThreadPriorityShift = 10,

#if STATS
		StatsThread_Local = StatsThread | LocalQueue,
#endif
		GameThread_Local = GameThread | LocalQueue,
		ActualRenderingThread_Local = ActualRenderingThread | LocalQueue,



	};

	FORCEINLINE Type setTaskPriority(Type ThreadAndIndex, Type TaskPriority)
	{
		BOOST_ASSERT((!(ThreadAndIndex & ~ThreadIndexMask) && !(TaskPriority & ~TaskPriorityMask)));
		return Type(ThreadAndIndex | TaskPriority);
	}


	extern Type RenderThread;
	extern Type RenderThread_Local;
	extern int32_t bHashBackgroundThreads;
	extern int32_t bHashHighPriorityThreads;
}


class GraphEvent
{

};

namespace Air
{

	typedef std::shared_ptr<class GraphEvent> GraphEventPtr;

class TaskGraphInterface
{
	friend class BaseGraphTask;
	virtual void queueTask(class BaseGraphTask* task, ENamedThreads::Type threadToExecuteOn, ENamedThreads::Type currentThreadIfKnown = ENamedThreads::AnyThread) = 0;

public:
	virtual ~TaskGraphInterface()
	{

	}
	static AIR_CORE_API void Startup(int32_t numThreads);

	static AIR_CORE_API void shutdown();

	static AIR_CORE_API bool isRunning();

	static AIR_CORE_API TaskGraphInterface& get();

	virtual ENamedThreads::Type getCurrentThreadIfKnown(bool bLocalQueue = false);

	virtual int32_t getNumWorkerThreads() = 0;

	virtual bool isThreadProcessingTasks(ENamedThreads::Type threadToCheck) = 0;



};




class AIR_CORE_API AutoConsoleTaskPriority
{
	std::string mCommandName;
	ENamedThreads::Type mThreadPriority;
	ENamedThreads::Type mTaskPriority;
	ENamedThreads::Type mTaskPriorityIfForcedToNormalThreadPriority;

	void commandExecute(const std::vector<std::string>& args);
public:
	AutoConsoleTaskPriority(const char* Name, const char* Help, ENamedThreads::Type DefaultThreadPriority, ENamedThreads::Type DefaultTaskPriority, ENamedThreads::Type DefaultTaskPriorityIfForcedToNormalThreadPriority = ENamedThreads::UnusedAnchor)
		: mCommandName(Name)
		, mThreadPriority(DefaultThreadPriority)
		, mTaskPriority(DefaultTaskPriority)
		, mTaskPriorityIfForcedToNormalThreadPriority(DefaultTaskPriorityIfForcedToNormalThreadPriority)
	{

	}


	FORCEINLINE ENamedThreads::Type get(ENamedThreads::Type thread = ENamedThreads::AnyThread)
	{
		if (mThreadPriority == ENamedThreads::HighThreadPriority && !ENamedThreads::bHashHighPriorityThreads)
		{
			return ENamedThreads::setTaskPriority(thread, mTaskPriorityIfForcedToNormalThreadPriority);
		}

		if (mThreadPriority == ENamedThreads::BackgroundThreadPriority && !ENamedThreads::bHashBackgroundThreads)
		{
			return ENamedThreads::setTaskPriority(thread, mTaskPriorityIfForcedToNormalThreadPriority);
		}
		return ENamedThreads::setTaskPriority(thread, mThreadPriority, mTaskPriority);
	}
};


}




#endif
