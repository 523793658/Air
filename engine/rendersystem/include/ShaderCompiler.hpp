#ifndef _ShaderCompiler_H_
#define _ShaderCompiler_H_
#pragma once
#include "basic/include/Thread.h"
#include "basic/include/PlatformProcess.h"
#include "rendersystem/shaderCore/ShaderCore.hpp"
#include "rendersystem/shaderCore/Shader.hpp"
namespace Air
{
	class ShaderCompileJob;
	class ShaderPipelineCompileJob;

	class ShaderCommonCompileJob
	{
	public:
		uint32_t mID;
		bool mFinalized;
		bool mSucceeded;
		bool mOptimizeForLowLatency;

		ShaderCommonCompileJob(uint32_t id)
			:mID(id),
			mFinalized(false),
			mSucceeded(false),
			mOptimizeForLowLatency(false)
		{

		}

		virtual ShaderCompileJob* getSingleShaderJob() { return nullptr; }
		virtual const ShaderCompileJob* getSingleShaderJob() const { return nullptr; }
		virtual ShaderPipelineCompileJob* getShaderPipelineJob() { return nullptr; }
		virtual const ShaderPipelineCompileJob* getShaderPipelineJob()const { return nullptr; }
	};

	class ShaderCompileJob : public ShaderCommonCompileJob
	{
	public:
		class VertexFactoryType* mVFType;
		class FShaderType* mShaderType;
		ShaderCompilerInput mInput;
		ShaderCompilerOutput mOutput;

		std::unordered_map<const class VertexFactoryType*, std::vector<const class ShaderPipelineType*>> mSharingPipelines;

		ShaderCompileJob(uint32_t id, class VertexFactoryType* VFType, FShaderType* shaderType):
			ShaderCommonCompileJob(id),
			mVFType(VFType),
			mShaderType(shaderType)
		{}

		virtual ShaderCompileJob* getSingleShaderJob() override { return this; }
		virtual const ShaderCompileJob* getSingleShaderJob() const override
		{
			return this;
		}

		friend Archive& operator << (Archive& ar, ShaderCompileJob& input);
	};

	class ShaderPipelineCompileJob : public ShaderCommonCompileJob
	{
	public:
		std::vector<ShaderCommonCompileJob*> mStageJobs;
		bool mFailedRemovingUnused;
		const ShaderPipelineType* mShaderPipeline;

		ShaderPipelineCompileJob(uint32_t id, const ShaderPipelineType* shaderPipeline, int32_t numStages) :
			ShaderCommonCompileJob(id),
			mFailedRemovingUnused(false),
			mShaderPipeline(shaderPipeline)
		{
			mStageJobs.reserve(numStages);
		}
		~ShaderPipelineCompileJob()
		{
			for (int32_t index = 0; index < mStageJobs.size(); ++index)
			{
				delete mStageJobs[index];
			}
			mStageJobs.clear();
		}

		virtual ShaderPipelineCompileJob* getShaderPipelineJob() override
		{
			return this;
		}
		virtual const ShaderPipelineCompileJob* getShaderPipelineJob() const override
		{
			return this;
		}

		friend Archive& operator << (Archive& ar, ShaderPipelineCompileJob& job);
	};

	class ShaderCompileThreadRunnableBase : public Runable
	{
		friend class ShaderCompilingManager;
	protected:
		class ShaderCompilingManager* mManager;
		std::unique_ptr<joiner<uint32_t>> mThread;

		std::string mErrorMessage;

		bool mTerminatedByError;

		volatile bool mForceFinish;
	public:
		ShaderCompileThreadRunnableBase(class ShaderCompilingManager* manager);
		virtual ~ShaderCompileThreadRunnableBase()
		{

		}

		void startThread();

		virtual void stop() { mForceFinish = true; }

		virtual uint32_t run();

		inline void waitForCompletion()
		{
			if (mThread)
			{
				mThread->detach();
				mThread.reset();
			}
		}

		void checkHealth() const;

		virtual int32_t compilingLoop() = 0;
	};



	class ShaderCompileThreadRunable : public ShaderCompileThreadRunnableBase
	{
		friend class ShaderCompilingManager;
	private:

	

		std::vector<struct ShaderCompileWorkerInfo*> mWorkerInfos;
		double mLastCheckForWorkersTime;

	public:
		ShaderCompileThreadRunable(class ShaderCompilingManager* manager);
		virtual ~ShaderCompileThreadRunable();

	private:
		int32_t pullTasksFromQueue();

		void writeNewTasks();

		bool launchWorkersIfNeeded();

		void readAvailableResult();

		void compileDirectlyThroughDll();

		virtual int32_t compilingLoop() override;

	};

	class ShaderCompileXGEThreadRunnable : public ShaderCompileThreadRunable
	{
	private:
		ProcHandle mBuildProcessHandle;

		uint32_t mBuildProcessID;


		class ShaderBatch
		{
			std::vector<class ShaderCommonCompileJob*> mJobs;
			bool mTransferFileWritten;

		public:
			const std::string& mDirectoryBase;
			const std::string& mInputFileName;
			const std::string& mSuccessFileName;
			const std::string& mOutputFileName;
			int32_t mBatchIndex;
			int32_t mDirectoryIndex;
			std::string mWorkingDirectory;
			std::string mOutputFileNameAndPath;
			std::string mSuccessFileNameAndPath;
			std::string mInputFileNameAndPath;

			ShaderBatch(const std::string& directoryBase, const std::string & inputFileName, const std::string& successFileName, const std::string outputFileName, int32_t directoryIndex, int32_t batchIndex)
				:mTransferFileWritten(false)
				,mDirectoryBase(directoryBase)
				,mInputFileName(inputFileName)
				,mSuccessFileName(successFileName)
				,mOutputFileName(outputFileName)
			{
				setIndices(directoryIndex, batchIndex);
			}

			void setIndices(int32_t directoryIndex, int32_t batchIndex);

			void cleanUpFiles(bool keepInputFile);

			inline int32_t getNumJobs()
			{
				return static_cast<int32_t>(mJobs.size());
			}
			inline const std::vector<class ShaderCommonCompileJob*>& getJobs() const
			{
				return mJobs;
			}
			void addJob(class ShaderCommonCompileJob* job);

			void writeTransferFile();
		};
		std::vector<ShaderBatch*> mShaderBatchesInFlight;
		std::vector<ShaderBatch*> mShaderBatchesFull;
		std::vector<ShaderBatch*> mShaderBatchesIncompile;

		const std::string mXGEWorkingDirectory;
		uint32_t mXGEDirectoryIndex;

		double lastAddTime;
		uint64_t mStartTime;
		int32_t mBatchIndexToCreate;
		int32_t mBatchIndexToFill;

		void postCompletedJobsForBatch(ShaderBatch* batch);

		void gatherResultsFromXGE();
	public:
		ShaderCompileXGEThreadRunnable(class ShaderCompilingManager* manager);

		virtual ~ShaderCompileXGEThreadRunnable();

		virtual int32_t compilingLoop() override;

		static bool isSupported();
	};

	struct ShaderMapCompileResults
	{
		ShaderMapCompileResults()
		{

		}

		int32_t mNumJobsQueued{ 0 };
		bool bAllJobsSucceeded{ true };
		bool bApplyCompletedShaderMapForRendering{ true };
		bool bRecreateComponentRenderStateOnCompletion{ false };

		std::vector<class ShaderCommonCompileJob*> mFinishedJobs;
	};


	struct ShaderMapFinalizeResults : public ShaderMapCompileResults
	{
		int32_t mFinalizeJobIndex;
		std::unordered_map<const class VertexFactoryType*, std::vector<const class ShaderPipelineType*>> mSharedPipelines;

		ShaderMapFinalizeResults(const ShaderMapCompileResults& compileResults)
			:
			ShaderMapCompileResults(compileResults),
			mFinalizeJobIndex(0)
		{

		}
	};

	class ShaderCompilingManager
	{
		friend class ShaderCompileThreadRunnableBase;
		friend class ShaderCompileThreadRunable;
		friend class ShaderCompileXGEThreadRunnable;
	private:
		bool mCompilingDuringGame;
		std::vector<class ShaderCommonCompileJob*> mCompileQueue;
		std::unordered_map<int32_t, ShaderMapCompileResults> mShaderMapJobs;
		int32_t mNumOutstandingJobs;

		std::mutex mQueueMutex;

		std::unordered_map<int32_t, ShaderMapFinalizeResults> mPendingFinalizeShaderMaps;

		std::unique_ptr<ShaderCompileThreadRunnableBase> mThread;

		uint32_t mNumShaderCompilingThreads;

		uint32_t mNumShaderCompilingThreadsDuringGame;

		int32_t mMaxShaderJobBatchSize;
		uint32_t mProcessId;

		bool mAllowCompilingThroughWorkers;

		bool mAllowAsynchronousShaderCompiling;

		bool mPromptToRetryFailedShaderCompiles;

		bool mLogJobCompletionTimes;

		float mProcessGameThreadTargetTime;

		std::string mShaderBaseWorkingDirectory;

		std::string mAbsoluteShaderBaseWorkingDirectory;

		std::string mAbsoluteShaderDebugInfoDirectory;

		std::string mShaderCompileWorkerName;

		bool mFallBackToDirectCompiles;

		double mWorkersBusyTime;

		uint64_t mSuppressedShaderPlatforms;

	public:
		ShaderCompilingManager();

		static bool isShaderCompilerWorkderRunning(ProcHandle& workerHandle);

		ProcHandle launchWorker(std::string & workingDirectory, uint32_t inProcessId, uint32_t threadId, const std::string & workerInputFile, const std::string & workderOuputFile);
	};


}
#endif
