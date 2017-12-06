#include "Context.h"
#include "Engine.h"
#include "ResLoader.h"
#include "basic/include/Thread.h"
#include "rendersystem/include/ShaderCompiler.hpp"
#include "basic/include/Log.hpp"
#include "basic/include/PlatformMisc.h"
#include "basic/include/Timer.hpp"
#include "rendersystem/include/RenderEngine.hpp"
#include <boost/lexical_cast.hpp>
#include "rendersystem/shaderCore/ShaderFormat.hpp"
#include "basic/include/ITargetPlatformManagerModule.h"
#include "basic/include/ModuleManager.h"
#include "Misc/CoreMisc.hpp"
#include "rendersystem/shaderCore/Shader.hpp"
#include "HAL/PlatformAtomics.h"
#include "Misc/GUID.hpp"
#include "Misc/CommandLine.hpp"
#include "HAL/FileSystem.h"
#include "serialization/Archive.hpp"
#include "rendersystem/shaderCore/ShaderFormat.hpp"
#include "rendersystem/shaderCore/ShaderFormatModule.hpp"
#include "basic/include/STLUnorderMap.hpp"
#include "HAL/PlatformFileSystem.h"
#include "GenericPlatform/GenericPlatformFile.h"
using namespace std::experimental;

extern bool GIsBuildMachine;

namespace Air
{
	const int32_t ShaderCompileWorkerInputVersion = 1;

	const int32_t ShaderCompileWorkerOutputVersion = 1;

	const int32_t ShaderCompileWorkerSingleJobHeader = 'S';

	const int32_t ShaderCompileWorkerPipelineJobHeader = 'P';

#define DEBUG_SHADERCOMPILEWORKER 0


	extern bool compileShaderPipeline(const IShaderFormat* compiler, std::string format, ShaderPipelineCompileJob* pipelineJob, const std::string_view dir);

	static const std::vector<const IShaderFormat*>& getShaderFormats()
	{
		static bool bInitialized = false;
		static std::vector<const IShaderFormat*> results;
		if (!bInitialized)
		{
			bInitialized = true;
			size_t size = results.size();
			results.clear();
			results.reserve(size);
			std::vector<std::string> modules;
			ModuleManager::get().findModules(SHADERFORMAT_MODULE_WILDCARD, modules);
			if (modules.size() == 0)
			{

			}
			for (int32_t index = 0; index < modules.size(); ++index)
			{
				IShaderFormat* format = ModuleManager::loadModuleChecked<IShaderFormatModule>(modules[index]).getShaderFormat();
				if (format != nullptr)
				{
					results.push_back(format);
				}
			}
		}
		return results;
	}

	static inline void getFormatVersionMap(std::unordered_map<std::string, uint16_t>& outFormatVersionMap)
	{
		if (outFormatVersionMap.size() == 0)
		{
			const std::vector<const class IShaderFormat*> & shaderFormats = getShaderFormats();
			BOOST_ASSERT(shaderFormats.size());
			for (int32_t index = 0; index < index < shaderFormats.size(); ++index)
			{
				std::vector<std::string> outFormats;
				shaderFormats[index]->getSupportedFormats(outFormats);
				BOOST_ASSERT(outFormats.size());
				for (int32_t innerIndex = 0; innerIndex < outFormats.size(); ++innerIndex)
				{
					uint16_t version = shaderFormats[index]->getVersion(outFormats[innerIndex]);
					outFormatVersionMap.emplace(outFormats[innerIndex], version);
				}
			}
		}
	}

	static void spiltJobsByType(const std::vector<ShaderCommonCompileJob*>& queuedJobs, std::vector<ShaderCompileJob*>& outQueuedSingleJobs, std::vector<ShaderPipelineCompileJob*>& outqueuedPipelineJobs)
	{
		for (auto job : queuedJobs)
		{
			auto* pipelineJob = job->getShaderPipelineJob();
			if (pipelineJob)
			{
				outqueuedPipelineJobs.push_back(pipelineJob);
			}
			else
			{
				auto* singleJob = job->getSingleShaderJob();
				BOOST_ASSERT(singleJob);
				outQueuedSingleJobs.push_back(singleJob);
			}
		}
	}
	static void doReadTaskResults(const std::vector<ShaderCommonCompileJob*>& queuedJobs, Archive& outputFile)
	{

	}

	static bool doWriteTasks(const std::vector<ShaderCommonCompileJob*>& queuedJobs, Archive& transferFile)
	{
		int32_t inputVersion = ShaderCompileWorkerInputVersion;
		transferFile << inputVersion;
		static std::unordered_map<std::string, uint16_t> formatVersionMap;
		getFormatVersionMap(formatVersionMap);
		transferFile << formatVersionMap;

		std::vector<ShaderCompileJob*> queuedSingleJobs;
		std::vector<ShaderPipelineCompileJob*> queuedPipelineJobs;
		spiltJobsByType(queuedJobs, queuedSingleJobs, queuedPipelineJobs);
		{
			int32_t singleJobHeader = ShaderCompileWorkerSingleJobHeader;
			transferFile << singleJobHeader;
			int32_t numBatches = queuedSingleJobs.size();
			transferFile << numBatches;
			for (int32_t jobIndex = 0; jobIndex < queuedSingleJobs.size(); ++jobIndex)
			{
				transferFile << queuedSingleJobs[jobIndex]->mInput;
			}
		}
		{
			int32_t pipelineJobHeader = ShaderCompileWorkerPipelineJobHeader;
			transferFile << pipelineJobHeader;
			int32_t numBatches = queuedPipelineJobs.size();
			transferFile << numBatches;
			for (auto job : queuedPipelineJobs)
			{
				std::wstring pipelineName = job->mShaderPipeline->getName();
				transferFile << pipelineName;
				int32_t numStageJobs = job->mStageJobs.size();
				transferFile << numStageJobs;
				for (auto stageJob : job->mStageJobs)
				{
					transferFile << stageJob->getSingleShaderJob()->mInput;
				}
			}
		}
		return transferFile.close();
	}


	namespace XGEConsoleVariables
	{
		int32_t enabled = 0;

		double JobTimeout = 0.5;
	}


	struct ShaderCompileWorkerInfo
	{
		ProcHandle mWorkerProcess;
		bool mIssuedTasksToWorker;

		bool mLaunchedWorker;

		bool mComplete;

		double mStartTime;

		std::vector<class ShaderCommonCompileJob*> mQueuedJobs;

		ShaderCompileWorkerInfo() :
			mIssuedTasksToWorker(false),
			mLaunchedWorker(false),
			mComplete(false),
			mStartTime(0)
		{

		}

		~ShaderCompileWorkerInfo()
		{
			if (mWorkerProcess.isValid())
			{
				PlatformProcess::terminateProc(mWorkerProcess);
				PlatformProcess::closeProc(mWorkerProcess);
			}
		}
	};


	static void moveFileHelper(const std::string& to, const std::string& from)
	{
	
		filesystem::path toPath(to);
		filesystem::path fromPath(from);

		if (filesystem::exists(fromPath))
		{
			bool success = false;
			INT32 retryCount = 0;
			while (!success && retryCount < 200)
			{
				if (retryCount > 0)
				{
					PlatformProcess::sleep(0.0f);
				}
				success = filesystem::create_directories(toPath);
				if (success)
				{
					success = filesystem::copy_file(fromPath, toPath, filesystem::copy_options::skip_existing);
				}
				retryCount++;
			}
			BOOST_ASSERT(success, "failed to move file");
		}
	}

	static ResIdentifierPtr createFileHelper(const std::string & pathName)
	{
		ResIdentifierPtr file;
		int32_t retryCount = 0;
		
	}


	void ShaderCompileThreadRunnableBase::startThread()
	{
		if (mManager->mAllowAsynchronousShaderCompiling)
		{
			mThread = MakeUniquePtr<joiner<uint32_t>>(Engine::getInstance().getThreadPool()(std::bind(&ShaderCompileThreadRunnableBase::run, this)));
		}
	}

	uint32_t ShaderCompileThreadRunnableBase::run()
	{
		if (!PlatformMisc::isDebuggerPresent())
		{
			BOOST_ASSERT(mManager->mAllowAsynchronousShaderCompiling);
			while (!mForceFinish)
			{
				compilingLoop();
			}
		}
		else
		{
			BOOST_ASSERT(mManager->mAllowAsynchronousShaderCompiling);
			while (!mForceFinish)
			{
				compilingLoop();
			}
		}
		return 0;
	}

	bool ShaderCompileThreadRunable::launchWorkersIfNeeded()
	{
		const double currentTime = Timer::getCurrentTime();
		const bool checkForWorkerRunning = (currentTime - mLastCheckForWorkersTime > 0.1f);
		bool abandonWorkers = false;
		if (checkForWorkerRunning)
		{
			mLastCheckForWorkersTime = currentTime;
		}
		for (int32_t workerIndex = 0; workerIndex < mWorkerInfos.size(); ++workerIndex)
		{
			ShaderCompileWorkerInfo& currentWorkerInfo = *mWorkerInfos[workerIndex];
			if (currentWorkerInfo.mQueuedJobs.size() == 0)
			{
				if (currentWorkerInfo.mWorkerProcess.isValid() && !ShaderCompilingManager::isShaderCompilerWorkderRunning(currentWorkerInfo.mWorkerProcess))
				{
					PlatformProcess::closeProc(currentWorkerInfo.mWorkerProcess);
					currentWorkerInfo.mWorkerProcess = ProcHandle();
				}
				continue;
			}
			if (!currentWorkerInfo.mWorkerProcess.isValid() || (checkForWorkerRunning && !ShaderCompilingManager::isShaderCompilerWorkderRunning(currentWorkerInfo.mWorkerProcess)))
			{
				bool launchAgain = true;
				if (currentWorkerInfo.mWorkerProcess.isValid())
				{
					PlatformProcess::closeProc(currentWorkerInfo.mWorkerProcess);
					currentWorkerInfo.mWorkerProcess = ProcHandle();
					if (currentWorkerInfo.mLaunchedWorker)
					{
						const std::string workingDirectory = mManager->mAbsoluteShaderBaseWorkingDirectory + boost::lexical_cast<std::string>(workerIndex) + "/";
						const std::string outputFileNameAndPath = workingDirectory + "WorkerOutputOnly.out";
						if (PlatformFileSystem::get().getPlatformFile().fileExist(outputFileNameAndPath))
						{
							launchAgain = false;
						}
						else
						{
							abandonWorkers = true;
							break;
						}
					}
				}
				if (launchAgain)
				{
					std::string workingDirectory = mManager->mShaderBaseWorkingDirectory + boost::lexical_cast<std::string>(workerIndex) + "/";
					std::string inputFileName = "WorkerInputOnly.in";
					std::string outputFileName = "WorkerOutputOnly.out";
					currentWorkerInfo.mWorkerProcess = mManager->launchWorker(workingDirectory, mManager->mProcessId, workerIndex, inputFileName, outputFileName);
					currentWorkerInfo.mLaunchedWorker = true;
				}
			}
		}
		return abandonWorkers;
	}


	void ShaderCompileThreadRunable::readAvailableResult()
	{
		for (int32_t workerIndex = 0; workerIndex < mWorkerInfos.size(); ++workerIndex)
		{
			ShaderCompileWorkerInfo& currentWorkerInfo = *mWorkerInfos[workerIndex];
			if (currentWorkerInfo.mQueuedJobs.size() > 0)
			{
				const std::string workingDirectory = mManager->mAbsoluteShaderBaseWorkingDirectory + boost::lexical_cast<std::string>(workerIndex) + "/";
				const std::string InputFileName = "WorkderInputOnly.in";
				const std::string outputFileNameAndPath = workingDirectory + "WorkerOutputOnly.out";
				if (PlatformFileSystem::get().getPlatformFile().fileExist(outputFileNameAndPath))
				{
					Archive* outputFilePtr = IFileSystem::get().createFileReader(outputFileNameAndPath, FILEREAD_Silent);
					if (outputFilePtr)
					{
						Archive& outputFile = *outputFilePtr;
						BOOST_ASSERT(!currentWorkerInfo.mComplete);
						doReadTaskResults(currentWorkerInfo.mQueuedJobs, outputFile);
						delete outputFilePtr;
						bool deletedOutput = IFileSystem::get().deleteFile(outputFileNameAndPath, true, true);
						int32_t retryCount = 0;
						while (!deletedOutput && retryCount < 200)
						{
							PlatformProcess::sleep(0.01f);
							deletedOutput = IFileSystem::get().deleteFile(outputFileNameAndPath, true, true);
							retryCount++;
						}
						BOOST_ASSERT(deletedOutput, "failed to delete");
						currentWorkerInfo.mComplete = true;
					}
				}
			}
		}
	}

	void ShaderCompileThreadRunable::writeNewTasks()
	{
		for (int32_t workerIndex = 0; workerIndex < mWorkerInfos.size(); ++workerIndex)
		{
			ShaderCompileWorkerInfo& currentWorkerInfo = *mWorkerInfos[workerIndex];
			if (!currentWorkerInfo.mIssuedTasksToWorker && currentWorkerInfo.mQueuedJobs.size() > 0)
			{
				currentWorkerInfo.mIssuedTasksToWorker = true;
				const std::string workingDirectory = mManager->mAbsoluteShaderBaseWorkingDirectory + boost::lexical_cast<std::string>(workerIndex);
				filesystem::path path;
				do 
				{
					Guid guid;
					PlatformMisc::createGuid(guid);
					path = workingDirectory + guid.toString();

				} while (filesystem::exists(path));
				Archive* transferFile = nullptr;
				int32_t retryCount = 0;
				while (transferFile == nullptr && retryCount < 2000)
				{
					if (retryCount > 0)
					{
						PlatformProcess::sleep(0.01f);

					}
					transferFile = IFileSystem::get().createFileWriter(path.c_str());
					retryCount++;
					if (transferFile == nullptr)
					{

					}
				}
				if (transferFile == nullptr)
				{
				}
				if (!doWriteTasks(currentWorkerInfo.mQueuedJobs, *transferFile))
				{
					uint64_t totalDiskSpace = 0;
					uint64_t freeDiskSpace = 0;
					PlatformMisc::getDiskTotalAndFreeSpace(path.generic_string(), totalDiskSpace, freeDiskSpace);
				}
				delete transferFile;

				std::string propertransferFileName = workingDirectory + "/WorkerInputOnly.in";
				if (!IFileSystem::get().move(propertransferFileName.c_str(), path.string().c_str()))
				{
					uint64_t TotalDiskSpace = 0;
					uint64_t freeDiskSpace = 0;
					PlatformMisc::getDiskTotalAndFreeSpace(path.generic_string(), TotalDiskSpace, freeDiskSpace);
				}
			}
		}
	}

	int32_t ShaderCompileThreadRunable::pullTasksFromQueue()
	{
		int32_t numActiveThread = 0;
		{
			std::lock_guard<std::mutex> lock(mManager->mQueueMutex);
			const int32_t numWorkersToFeed = mManager->mCompilingDuringGame ? mManager->mNumShaderCompilingThreadsDuringGame : mWorkerInfos.size();
			for (int32_t workerIndex = 0; workerIndex < mWorkerInfos.size(); ++workerIndex)
			{
				ShaderCompileWorkerInfo& currentWrokerInfo = *mWorkerInfos[workerIndex];
				if (currentWrokerInfo.mQueuedJobs.size() == 0 && workerIndex  < numWorkersToFeed)
				{
					BOOST_ASSERT(!currentWrokerInfo.mComplete);
					if (mManager->mCompileQueue.size() > 0)
					{
						bool bAddedLowLatencyTask = false;
						int32_t JobIndex = 0;
						for (; JobIndex < mManager->mMaxShaderJobBatchSize && JobIndex < mManager->mCompileQueue.size() && !bAddedLowLatencyTask; JobIndex++)
						{
							bAddedLowLatencyTask != mManager->mCompileQueue[JobIndex]->mOptimizeForLowLatency;
							currentWrokerInfo.mQueuedJobs.push_back(mManager->mCompileQueue[JobIndex]);
						}
						currentWrokerInfo.mIssuedTasksToWorker = false;
						currentWrokerInfo.mLaunchedWorker = false;
						numActiveThread++;
						mManager->mCompileQueue.erase(mManager->mCompileQueue.begin(), mManager->mCompileQueue.begin() + JobIndex);

					}
				}
				else
				{
					if (currentWrokerInfo.mQueuedJobs.size() > 0)
					{
						numActiveThread++;
					}
					if (currentWrokerInfo.mComplete)
					{
						for (int32_t jobIndex = 0; jobIndex < currentWrokerInfo.mQueuedJobs.size(); ++jobIndex)
						{
							auto it = mManager->mShaderMapJobs.find(currentWrokerInfo.mQueuedJobs[jobIndex]->mID);
							if (it != mManager->mShaderMapJobs.end())
							{
								ShaderMapCompileResults& shaderMapResults =
									it->second;
								shaderMapResults.mFinishedJobs.push_back(currentWrokerInfo.mQueuedJobs[jobIndex]);
								shaderMapResults.bAllJobsSucceeded = shaderMapResults.bAllJobsSucceeded && currentWrokerInfo.mQueuedJobs[jobIndex]->mSucceeded;
							}

							const float elapsedTime = Timer::getCurrentTime() - currentWrokerInfo.mStartTime;
							mManager->mWorkersBusyTime += elapsedTime;

							PlatformAtomics::InterlockedAdd(&mManager->mNumOutstandingJobs, -currentWrokerInfo.mQueuedJobs.size());
							currentWrokerInfo.mComplete = false;
							currentWrokerInfo.mQueuedJobs.clear();
						}
					}
				}
			}
			return numActiveThread;
		}
	}

	void ShaderCompileThreadRunable::compileDirectlyThroughDll()
	{
		for (int32_t workerIndex = 0; workerIndex < mWorkerInfos.size(); ++workerIndex)
		{
			ShaderCompileWorkerInfo& currentWorkerInfo = *mWorkerInfos[workerIndex];
			if (currentWorkerInfo.mQueuedJobs.size() > 0)
			{
				for (int32_t jobIndex = 0; jobIndex < currentWorkerInfo.mQueuedJobs.size(); ++jobIndex)
				{
					ShaderCommonCompileJob& currentJob = *currentWorkerInfo.mQueuedJobs[jobIndex];
					BOOST_ASSERT(!currentJob.mFinalized);
					currentJob.mFinalized = true;

					static ITargetPlatformManagerModule& TPM = getTargetPlatformManagerRef();

					auto* singleJob = currentJob.getSingleShaderJob();
					if (singleJob)
					{
						const std::string format = legacyShaderPlatformToShaderFormat(EShaderPlatform(singleJob->mInput.mTarget.mPlatform));
						const IShaderFormat* compiler = TPM.findShaderFormat(format);
						if (!compiler)
						{

						}
						BOOST_ASSERT(compiler != nullptr);
						if (singleJob->mInput.mSharedEnvironment)
						{
							singleJob->mInput.mEnvironment.merge(*singleJob->mInput.mSharedEnvironment);
						}
						compiler->compileShader(format, singleJob->mInput, singleJob->mOutput, PlatformProcess::shaderDir());
						singleJob->mSucceeded = singleJob->mOutput.mSucceeded;
						if (singleJob->mOutput.mSucceeded)
						{
							singleJob->mOutput.generateOutputHash();
						}
					}
					else
					{
						auto * pipelineJob = currentJob.getShaderPipelineJob();
						BOOST_ASSERT(pipelineJob);
						EShaderPlatform platform = (EShaderPlatform)pipelineJob->mStageJobs[0]->getSingleShaderJob()->mInput.mTarget.mPlatform;

						const std::string format = legacyShaderPlatformToShaderFormat(platform);
						const IShaderFormat* compiler = TPM.findShaderFormat(format);
						if (!compiler)
						{
						}
						BOOST_ASSERT(compiler != nullptr);
						for (int32_t index = 1; index < pipelineJob->mStageJobs.size(); ++index)
						{
							auto * singleState = pipelineJob->mStageJobs[index]->getSingleShaderJob();
							if (!singleState)
							{

							}
							if (platform != singleState->mInput.mTarget.mPlatform)
							{

							}
						}
						compileShaderPipeline(compiler, format, pipelineJob, PlatformProcess::shaderDir());
					}
				}
				currentWorkerInfo.mComplete = true;
			}
		}
	}

	int32_t ShaderCompileThreadRunable::compilingLoop()
	{
		const int32_t NumActiveThreads = pullTasksFromQueue();
		if (NumActiveThreads == 0 && mManager->mAllowAsynchronousShaderCompiling)
		{
			PlatformProcess::sleep(0.01f);
		}
		if (mManager->mAllowCompilingThroughWorkers)
		{
			writeNewTasks();
			bool bAbandonWorkers = launchWorkersIfNeeded();
			if (bAbandonWorkers)
			{
				mManager->mAllowCompilingThroughWorkers = false;
			}
			else
			{
				readAvailableResult();
			}
		}
		else
		{
			compileDirectlyThroughDll();
		}
		return NumActiveThreads;
	}


	static std::string XGE_ConsolePath;
	static const std::string XGE_SriptFileName("xgsript.xml");
	static const std::string XGE_InputFileName("WorkerInput.in");
	static const std::string XGE_OutputFileName("WorkerOutput.out");
	static const std::string XGE_SuccessFileName("Success");

	int32_t ShaderCompileXGEThreadRunnable::compilingLoop()
	{
		/*bool bWorkRemaining = false;
		if (mBuildProcessHandle.isValid())
		{
			gatherResultsFromXGE();
			bool bDoExitCheck = false;
			if (PlatformProcess::isProcRunning(mBuildProcessHandle))
			{
				if (mShaderBatchesInFlight.size() == 0)
				{
					PlatformProcess::waitForProc(mBuildProcessHandle);
					bDoExitCheck = true;
				}
			}
			else
			{
				bDoExitCheck = true;
			}
			if (bDoExitCheck)
			{
				if (mShaderBatchesInFlight.size() > 0)
				{
					gatherResultsFromXGE();
				}

				int32_t returnCode = 0;
				PlatformProcess::getProcReturnCode(mBuildProcessHandle, &returnCode);
				switch (returnCode)
				{
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:
					break;
				default:
					break;
				}
				for (ShaderBatch* batch : mShaderBatchesInFlight)
				{
					batch->cleanUpFiles(true);
					mShaderBatchesFull.push_back(batch);
					std::string oldInputFileName = batch->mInputFileNameAndPath;
					batch->setIndices(mXGEDirectoryIndex, mBatchIndexToCreate++);
					moveFileHelper(batch->mInputFileNameAndPath, oldInputFileName);
				}
				mShaderBatchesInFlight.clear();
				PlatformProcess::closeProc(mBuildProcessHandle);
			}
			else
			{
				bool buildDelayElapsed = ((Timer::getCurrentTime() - lastAddTime) >= XGEConsoleVariables::JobTimeout);
				bool hasJobsToRun = (mShaderBatchesIncompile.size() > 0 || mShaderBatchesFull.size() > 0);
				if (buildDelayElapsed && hasJobsToRun && mShaderBatchesInFlight.size() == 0)
				{
					mShaderBatchesInFlight.reserve(mShaderBatchesIncompile.size() + mShaderBatchesFull.size());
					for (ShaderBatch* batch : mShaderBatchesIncompile)
					{
						BOOST_ASSERT(batch->getNumJobs() > 0);
						batch->writeTransferFile();
						mShaderBatchesInFlight.push_back(batch);
					}

					for (ShaderBatch* batch : mShaderBatchesFull)
					{
						BOOST_ASSERT(batch->getNumJobs() > 0);
						mShaderBatchesInFlight.push_back(batch);
					}
					mShaderBatchesFull.clear();
					mShaderBatchesIncompile.clear();

					std::string scriptFileName = mXGEWorkingDirectory + boost::lexical_cast<std::string>(mXGEDirectoryIndex) + XGE_SuccessFileName;
					using namespace std::experimental;
					filesystem::path(scriptFileName);

				}
			}
		}*/
		return 0;
	}

	bool ShaderCompileXGEThreadRunnable::isSupported()
	{
		return false;
	}


	ShaderCompilingManager::ShaderCompilingManager():
		mCompilingDuringGame(false),
		mNumOutstandingJobs(0),
#ifdef AIR_PLATFORM_MAC
		mShaderCompileWorkerName(TEXT("../../bin/Mac/ShaderCompileWorker")),
#elif defined(AIR_PLATFORM_LINUX)
		mShaderCompileWorkerName(TEXT("../../bin/Linux/ShaderCompileWorker"))
#else
		mShaderCompileWorkerName("../../bin/x64/ShaderCompileWorker.exe"),
#endif
		mSuppressedShaderPlatforms(0)
	{
		mWorkersBusyTime = 0;
		mFallBackToDirectCompiles = false;
		mShaderCompileWorkerName = ResLoader::getInstance().getAbsPath(mShaderCompileWorkerName);
		if (!PlatformProcess::supportsMultithreading())
		{
			mAllowCompilingThroughWorkers = false;
		}

		if (!PlatformProcess::supportsMultithreading())
		{
			mAllowAsynchronousShaderCompiling = false;
		}

		int32_t numUnusedShaderCompilingThreads;


		mProcessId = PlatformProcess::getCurrentProcessId();

		const int32_t numVirtualCores = PlatformMisc::getNumberOfCoresIncludingHyperThreads();

		mNumShaderCompilingThreads = mAllowCompilingThroughWorkers ? (numVirtualCores - numUnusedShaderCompilingThreads) : 1;

		mNumShaderCompilingThreadsDuringGame = mAllowCompilingThroughWorkers ? (numVirtualCores - mNumShaderCompilingThreadsDuringGame) : 1;

		if (numVirtualCores <= 4)
		{
			mNumShaderCompilingThreads = numVirtualCores - 1;
			mNumShaderCompilingThreadsDuringGame = numVirtualCores - 1;
		}

		mNumShaderCompilingThreads = std::max<int32_t>(1, mNumShaderCompilingThreads);

		mNumShaderCompilingThreadsDuringGame = std::max<int32_t>(1, mNumShaderCompilingThreadsDuringGame);

		mNumShaderCompilingThreadsDuringGame = std::min<int32_t>(mNumShaderCompilingThreadsDuringGame, mNumShaderCompilingThreads);

		if (ShaderCompileXGEThreadRunnable::isSupported())
		{
			logInfo("Using XGE Shader Compiler.");
		}
		else
		{
			logInfo("Using Local Shader Compiler.");
		}
		mThread->startThread();
	}

	bool ShaderCompilingManager::isShaderCompilerWorkderRunning(ProcHandle& workerHandle)
	{
		return PlatformProcess::isProcRunning(workerHandle);
	}

	ProcHandle ShaderCompilingManager::launchWorker(std::string & workingDirectory, uint32_t inProcessId, uint32_t threadId, const std::string & workerInputFile, const std::string & workderOuputFile)
	{
		std::string workerAbsoluteDirectory = IFileSystem::get().convertToAbsolutePathForExternalAppForWrite(workingDirectory);
		std::string workerParameters = "\"" + workerAbsoluteDirectory + "/\" " + boost::lexical_cast<std::string>(inProcessId) + " " + boost::lexical_cast<std::string>(threadId) + " " + workerInputFile + " " + workderOuputFile;
		workerParameters += " -communicatethroughfile";
		if (GIsBuildMachine)
		{
			workerParameters += " -buildmachine ";
		}
		workerParameters += CommandLine::getSubprocessCommandline();
		int32_t priorityModifier = -1;
		if (DEBUG_SHADERCOMPILEWORKER)
		{
			const std::string & workerParametersText = workerParameters;
		}
		else
		{
			uint32_t workerId = 0;
			ProcHandle workerHandle = PlatformProcess::createProc(mShaderCompileWorkerName, workerParameters, true, false, false, &workerId, priorityModifier, NULL, NULL);
			if (!workerHandle.isValid())
			{
				BOOST_ASSERT(false, "couldn't launch compiler process");
			}
			return workerHandle;
		}
	}
}