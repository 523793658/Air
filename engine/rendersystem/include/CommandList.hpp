#ifndef _CommandList_H_
#define _CommandList_H_
#pragma once
#include "rendersystem/include/GraphicsBuffer.hpp"
#include "basic/include/ThreadSafeCounter.hpp"
#include "rendersystem/include/RenderLayout.hpp"
#include "core/include/CoreGlobals.hpp"
#include "rendersystem/include/TaskGraphInterfaces.hpp"

namespace Air
{
	class AIR_CORE_API CommandListBase : boost::noncopyable
	{
	public:
		mutable uint32_t mStrictGraphicsPipelineStateUse;

		CommandListBase();
		~CommandListBase();

		bool isImmediate();

		void waitForTasks(bool bKnownToBeComplete = false);

		void waitForDispatch();

		void waitForRHIThreadTasks();

		FORCEINLINE bool hasCommands() const
		{
			return (mNumCommands > 0);
		}

		void * operator new(size_t size);

		void operator delete(void * rawMenory);

	public:
		enum class ERenderThreadContext
		{
			SceneRenderTargets,
			Num
		};

		struct DrawUpData
		{
			uint32_t mPrimitiveType;
			uint32_t mNumPrimitives;
			uint32_t mNumVertices;
			uint32_t mVertexDataStride;
			void* mOutVertexData;
			uint32_t mMinVertexIndex;
			uint32_t mNumIndices;
			uint32_t mIndexDataStride;
			void* mOutIndexData;

			DrawUpData()
				:mPrimitiveType(TT_UNKNOWN),
				mOutVertexData(nullptr),
				mOutIndexData(nullptr)
			{

			}
		};


		FORCEINLINE void* getRenderThreadContext(ERenderThreadContext slot)
		{
			return mRenderThreadContexts[int32_t(slot)];
		}

	private:
		void *mRenderThreadContexts[(int32_t)ERenderThreadContext::Num];

		uint32_t mNumCommands;
	protected:

		DrawUpData mDrawUpData;
	};

	class AIR_CORE_API CommandList : public CommandListBase
	{
	public:

	};

	namespace EImmediateFlushType
	{
		enum Type
		{
			WaitForOutstandingTasksOnly = 0,
			DispatchToRHIThread,
			WaitForDispatchToRHIThread,
			FlushRHIThread,
			FlushRHIThreadFlushResources
		};
	}


	class AIR_CORE_API CommandListImmediate : public CommandList
	{
	public:
		void immediateFlush(EImmediateFlushType::Type flushType);
	};

	class AIR_CORE_API CommandListExecutor
	{
	public:
		enum
		{
			DefualtBypass = 0
		};

		CommandListExecutor()
		{

		}
		static inline CommandListImmediate& getImmediateCommandList();

		void executeList(CommandListBase& cmdList);
		void exxcuteList(CommandListImmediate& cmdlist);
		void latchbypass();

		static void waitOnRenderThreadFence(GraphEventPtr& fence);
	private:

		void executeInner(CommandListBase& cmdList);
		friend class ExecuteRHIThreadTask;
		static void executeInner_DoExecute(CommandListBase& cmdList);

		friend class CommandListBase;
		ThreadSafeCounter UIDCounter;
		ThreadSafeCounter OutStandingCmdListCount;
		CommandListImmediate mCommandListImmediate;
	};

	extern AIR_CORE_API CommandListExecutor GRHICommandList;
	extern AIR_CORE_API AutoConsoleTaskPriority CPrio_SceneRenderingTask;



	class RenderTask
	{

		FORCEINLINE static ENamedThreads::Type getDesiredThread()
		{
			return CPrio_SceneRenderingTask.get();
		}



	};

}




#include "rendersystem/include/CommandList.inl"

#endif
