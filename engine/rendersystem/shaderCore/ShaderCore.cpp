#include "Context.h"
#include "rendersystem/shaderCore/ShaderCore.hpp"
#include "basic/include/STLVector.hpp"
#include "basic/include/STLUnorderMap.hpp"

namespace Air
{
	void ShaderCompilerOutput::generateOutputHash()
	{
		size_t hash = 0;
		const std::vector<uint8_t>& code = mShaderCode.getReadAccess();
		uint32_t shaderCodeSize = mShaderCode.getShaderCodeSize();
		hash = boost::hash_range(code.begin(), code.end());
		mParameterMap.updateHash(hash);
		mHash = hash;
	}

	Archive& operator << (Archive & ar, ShaderCompilerEnvironment const & env)
	{
		ar << env.mIncludeFileNameToContentsMap.size();
		for (auto it : env.mIncludeFileNameToContentsMap)
		{
			ar << it.first;
			ar << it.second;
		}
		ar << env.mDefinitions << env.mCompilerFlags << env.mRenderTargetOutputFormatsMap << env.mResourceTableMap << env.mResourceTableLayoutHashes;
		return ar;
	}

	Archive & operator << (Archive& ar, ShaderCompilerDefinitions const & defin)
	{
		ar << defin.mDefinitions;
		return ar;
	}

	Archive& operator << (Archive& ar, ShaderCompilerInput& input)
	{
		ar << input.mTarget;
		{
			ar << input.mShaderFormat;
		}
		ar << input.mSourceFileProfix;
		ar << input.mSourceFileName;
		ar << input.mEntryPointName;
		ar << input.mSkipPreprocessedCache;
		ar << input.mCompilingForShaderPipeline;
		ar << input.mGenerateDirectCompileFile;
		ar << input.mIncludeUsedOutputs;
		ar << input.mUsedOutputs;
		ar << input.mDumpDebugInfoRootPath;
		ar << input.mDumpDebugInfoPath;
		ar << input.mDebugGroupName;
		ar << input.mEnvironment;
		return ar;
	}



}