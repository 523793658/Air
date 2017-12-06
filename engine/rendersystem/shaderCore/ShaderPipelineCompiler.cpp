#include "Context.h"
#include "basic/include/STLVector.hpp"
#include "rendersystem/shaderCore/ShaderFormat.hpp"
#include "rendersystem/include/ShaderCompiler.hpp"

namespace Air
{
	bool compileShaderPipeline(const IShaderFormat* compiler, std::string format, ShaderPipelineCompileJob* pipelineJob, const std::string_view dir)
	{
		BOOST_ASSERT(pipelineJob && pipelineJob->mStageJobs.size() > 0);
		ShaderCompileJob* currentJob = pipelineJob->mStageJobs[0]->getSingleShaderJob();

		currentJob->mInput.mCompilingForShaderPipeline = true;
		currentJob->mInput.mIncludeUsedOutputs = false;

		if (currentJob->mInput.mSharedEnvironment)
		{
			currentJob->mInput.mEnvironment.merge(*currentJob->mInput.mSharedEnvironment);
		}
		compiler->compileShader(format, currentJob->mInput, currentJob->mOutput, dir);
		currentJob->mSucceeded = currentJob->mOutput.mSucceeded;
		if (!currentJob->mOutput.mSucceeded)
		{
			return false;
		}
		currentJob->mOutput.generateOutputHash();
		bool enableRemovingUnused = true;
		bool jobFailedRemovingUnused = false;
		for (int32_t index = 0; index < pipelineJob->mStageJobs.size(); ++index)
		{
			auto stage = pipelineJob->mStageJobs[index]->getSingleShaderJob()->mInput.mTarget.mFrequency;
			if (stage != SF_Vertex && stage != SF_Pixel)
			{
				enableRemovingUnused = false;
				break;
			}
		}
		for (int32_t index = 1; index < pipelineJob->mStageJobs.size(); ++index)
		{
			auto* previousJob = currentJob;
			currentJob = pipelineJob->mStageJobs[index]->getSingleShaderJob();
			enableRemovingUnused = enableRemovingUnused && previousJob->mOutput.mSupportsQueryingUsedAttributes;
			if (enableRemovingUnused)
			{
				currentJob->mInput.mIncludeUsedOutputs = true;
				currentJob->mInput.mCompilingForShaderPipeline = true;
				currentJob->mInput.mUsedOutputs = previousJob->mOutput.mUsedAttributes;
			}
			if (currentJob->mInput.mSharedEnvironment)
			{
				currentJob->mInput.mEnvironment.merge(*currentJob->mInput.mSharedEnvironment);
			}
			compiler->compileShader(format, currentJob->mInput, currentJob->mOutput, dir);
			currentJob->mSucceeded = currentJob->mOutput.mSucceeded;
			if (!currentJob->mOutput.mSucceeded)
			{
				return false;
			}
			jobFailedRemovingUnused = currentJob->mOutput.mFailedRemovingUnused || jobFailedRemovingUnused;
			currentJob->mOutput.generateOutputHash();
		}
		pipelineJob->mSucceeded = true;
		pipelineJob->mFailedRemovingUnused = jobFailedRemovingUnused;
		return true;
	}
}