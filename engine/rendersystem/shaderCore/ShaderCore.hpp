#ifndef _ShaderCore_H_
#define _ShaderCore_H_
#pragma once
#include "rendersystem/include/RHIDefinitions.hpp"
#include "rendersystem/include/ShaderObject.hpp"
#include "rendersystem/include/UniformBuffer.hpp"
#include "basic/include/ResIdentifier.h"
#include "boost/lexical_cast.hpp"
#include "serialization/Archive.hpp"
namespace Air
{

struct ShaderTarget
{
	uint32_t mFrequency{ SF_NumBits };
	uint32_t mPlatform{ SP_NumBits };

	ShaderTarget()
	{}
	ShaderTarget(EShaderFrequency frequency, EShaderPlatform platform)
		:mFrequency(frequency),
		mPlatform(platform)
	{}
	friend bool operator == (const ShaderTarget & x, const ShaderTarget & y)
	{
		return x.mFrequency == y.mFrequency && x.mPlatform == y.mPlatform;
	}

	friend class Archive& operator << (Archive& file, ShaderTarget& target)
	{
		uint32_t targetFrequency = target.mFrequency;
		uint32_t targetPlatform = target.mPlatform;
		file << targetFrequency;
		file << targetPlatform;
	}
};

class ShaderCompilerDefinitions
{
public:
	ShaderCompilerDefinitions()
	{
		mDefinitions.reserve(50);
	}

	template<typename T>
	void setDefine(const std::string_view name, T value)
	{
		mDefinitions.emplace(name, boost::lexical_cast<std::string>(value));
	}

	const std::unordered_map<std::string, std::string>& getDefinitionMap() const
	{
		return mDefinitions;
	}

	void merge(const ShaderCompilerDefinitions & other)
	{
		for (auto it : other.mDefinitions)
		{
			mDefinitions.emplace(it);
		}
	}

private:
	std::unordered_map<std::string, std::string> mDefinitions;

};

class ShaderParameterMap
{
public:
	ShaderParameterMap()
	{}
	bool findParameterAllocation(std::string_view parameterName, uint16_t& outBufferIndex, uint16_t& outBaseIndex, uint16_t& outSize) const;

	bool containsParameterAllocation(std::string_view ParameterName) const;

	void addParameterAllocation(const std::string_view parameterName, uint16_t bufferIndex, uint16_t baseIndex, uint16_t size);
	void removeParameterAllocation(const std::string_view parameterName);

	void verifyBindingsArcComplete(const std::string_view ShaderTypeName, ShaderTarget target, class VertexFactoryType* vertexFactoryType) const;

	void updateHash(size_t& hashValue) const;

private:
	struct ParameterAllocation
	{
		uint16_t mBufferIndex;
		uint16_t mBaseIndex;
		uint16_t mSize;
		mutable bool mBound;
		ParameterAllocation()
			:mBound(false)
		{}

	};
	std::unordered_map<std::string, ParameterAllocation> mParametrMap;
};




struct ShaderCompilerEnvironment 
{
	std::unordered_map<std::string, std::vector<std::string>> mIncludeFileNameToContentsMap;
	std::vector<uint32_t> mCompilerFlags;
	std::unordered_map<uint32_t, uint8_t> mRenderTargetOutputFormatsMap;
	std::unordered_map<std::string, ResourceTableEntry> mResourceTableMap;
	std::unordered_map<std::string, uint32_t> mResourceTableLayoutHashes;

	ShaderCompilerEnvironment()
	{
		mIncludeFileNameToContentsMap.reserve(15);
	}

	explicit ShaderCompilerEnvironment(const ShaderCompilerDefinitions& definitions)
		:mDefinitions(definitions)
	{}
	template<typename T>
	void setDefine(std::string_view name, T value)
	{
		mDefinitions.setDefine(name, value);
	}

	const std::unordered_map<std::string, std::string>& getDefinitions() const
	{
		return mDefinitions.getDefinitionMap();
	}

	void setShaderTargetOutputFormat(uint32_t renderTargetIndex, ElementFormat format)
	{
		mRenderTargetOutputFormatsMap.emplace(renderTargetIndex, format);
	}

	void merge(const ShaderCompilerEnvironment& other)
	{
		for (auto it : other.mIncludeFileNameToContentsMap)
		{
			auto r = mIncludeFileNameToContentsMap.find(it.first);
			if (r != mIncludeFileNameToContentsMap.end())
			{
				if (r->second.size() > 0)
				{
					r->second.erase(r->second.end());
				}
				r->second.insert(r->second.end(), it.second.begin(), it.second.end());
			}
			else
			{
				mIncludeFileNameToContentsMap.emplace(it.first, it.second);
			}
		}
		mCompilerFlags.insert(mCompilerFlags.end(), other.mCompilerFlags.begin(), other.mCompilerFlags.end());

		mResourceTableMap.insert(other.mResourceTableMap.begin(), other.mResourceTableMap.end());

		mResourceTableLayoutHashes.insert(other.mResourceTableLayoutHashes.begin(), other.mResourceTableLayoutHashes.end());

		mDefinitions.merge(other.mDefinitions);
		mRenderTargetOutputFormatsMap.insert(other.mRenderTargetOutputFormatsMap.begin(), other.mRenderTargetOutputFormatsMap.end());
	}
private:
	ShaderCompilerDefinitions mDefinitions;
};

struct ShaderCompilerError
{
	ShaderCompilerError(const std::string_view strippedErrorMessage = "")
		:mErrorFile(""),
		mErrorLineString(""),
		mStrippedErrorMessage(strippedErrorMessage)
	{

	}
	std::string mErrorFile;
	std::string mErrorLineString;
	std::string mStrippedErrorMessage;
	std::string getErrorString() const
	{
		return mErrorFile + "(" + mErrorLineString + "): " + mStrippedErrorMessage;
	}
};

class ShaderCodeReader
{
	const std::vector<uint8_t>& mShaderCode;
public:
	ShaderCodeReader(const std::vector<uint8_t>& shaderCode)
		:mShaderCode(shaderCode)
	{
		BOOST_ASSERT(shaderCode.size());
	}

	int32_t getOptionalDataSize() const
	{
		if (mShaderCode.size() < sizeof(int32_t))
		{
			return 0;
		}
		const uint8_t* end = &mShaderCode[0] + mShaderCode.size();
		int32_t localOptionalDataSize = ((const uint32_t*)end)[-1];
		BOOST_ASSERT(localOptionalDataSize >= 0);
		BOOST_ASSERT(mShaderCode.size() > localOptionalDataSize);
		return localOptionalDataSize;
	}

	int32_t getShaderCodeSize() const
	{
		return static_cast<int32_t>(mShaderCode.size()) - getOptionalDataSize();
	}
};

class ShaderCode
{
	mutable int32_t mOptionalDataSize;
	mutable std::vector<uint8_t> mShaderCodeWithOptionalData;
public:
	ShaderCode()
		:mOptionalDataSize(0)
	{}
	void finalizeShaderCode() const
	{
		if (mOptionalDataSize != -1)
		{
			mOptionalDataSize += sizeof(mOptionalDataSize);
			size_t position = mShaderCodeWithOptionalData.size();
			mShaderCodeWithOptionalData.resize(mShaderCodeWithOptionalData.size() + sizeof(mOptionalDataSize));
			memcpy(&mShaderCodeWithOptionalData[position], &mOptionalDataSize, sizeof(mOptionalDataSize));
			mOptionalDataSize = -1;
		}
	}
	std::vector<uint8_t>& getWriteAccess()
	{
		return mShaderCodeWithOptionalData;
	}

	const std::vector<uint8_t>& getReadAccess() const
	{
		finalizeShaderCode();
		return mShaderCodeWithOptionalData;
	}

	int32_t getShaderCodeSize() const
	{
		finalizeShaderCode();
		ShaderCodeReader wrapper(mShaderCodeWithOptionalData);
		return wrapper.getShaderCodeSize();
	}

};


struct ShaderCompilerInput
{
	ShaderTarget mTarget;
	std::string mShaderFormat;
	std::string mSourceFileProfix;
	std::string mSourceFileName;
	std::string mEntryPointName;
	bool mSkipPreprocessedCache;
	bool mGenerateDirectCompileFile;
	bool mCompilingForShaderPipeline;
	bool mIncludeUsedOutputs;
	std::vector<std::string> mUsedOutputs;
	std::string mDumpDebugInfoRootPath;
	std::string mDumpDebugInfoPath;
	std::string mDebugGroupName;

	ShaderCompilerEnvironment mEnvironment;
	std::shared_ptr<ShaderCompilerEnvironment> mSharedEnvironment;

	friend Archive& operator << (Archive& ar, ShaderCompilerInput& input);
};


struct ShaderCompilerOutput
{
	ShaderParameterMap mParameterMap;
	std::vector<ShaderCompilerError> mErrors;
	ShaderTarget mTarget;
	ShaderCode mShaderCode;
	uint32_t mNumInstructions;
	uint32_t mNumTextureSamplers;

	size_t mHash;
	bool mSucceeded;
	bool mFailedRemovingUnused;
	bool mSupportsQueryingUsedAttributes;
	std::vector<std::string> mUsedAttributes;

	void generateOutputHash();
	
	friend Archive& operator << (Archive& ar, ShaderCompilerOutput& output);
};

}


#endif
