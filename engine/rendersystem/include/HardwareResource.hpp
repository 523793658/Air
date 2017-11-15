#ifndef _HardwareResource_H_
#define _HardwareResource_H_
#pragma once
namespace Air
{
	class AIR_CORE_API RHIResource
	{
	public:
		RHIResource(bool inbDoNotDeferDelete);

		static void flushPendingDeletes();
	};

	class RHISamplerState : public RHIResource {};

	class RHIRasterizerState : public RHIResource {};

	class RHIDepthStencilState : public RHIResource {};

	class RHIBlendState : public RHIResource {};

	class RHIVertexDeclaration : public RHIResource {};

	class RHIBoundShaderState : public RHIResource {};

	class RHIShader : public RHIResource
	{
		RHIShader(bool inbDoNotDeferDelete = false) : RHIResource(inbDoNotDeferDelete) {}

		void setHash(size_t inHash) { mHash = inHash; }
		size_t getHash() const { return mHash; }

	private:
		size_t mHash;
	};

	class AIR_CORE_API RHITexture : public RHIResource
	{
	public:

	};

	class RHIVertexShader : public RHIShader {};
	class RHIHullShader : public RHIShader {};
	class RHIDomainShader : public RHIShader {};
	class RHIPixelShader : public RHIShader {};
	class RHIGeometryShader : public RHIShader {};
	class RHIComputeShader : public RHIShader {};


	class RHIGraphicsPipelineState : public RHIResource {};
	class RHIComputePipelineState : public RHIResource {};
	struct RHIUniformBufferLayout
	{

		size_t getHash() const
		{
			if (!mBComputedHash)
			{
				mHashs = boost::hash_value(mConstantBufferSize);
				boost::hash_combine(mHashs, mResourceOffset);
				boost::hash_range(mHashs, mResource.begin(), mResource.end());
			}
			return mHashs;
		}
		explicit RHIUniformBufferLayout(std::string name)
			:mName(name)
		{

		}

		enum EInit
		{
			Zero
		};
		explicit RHIUniformBufferLayout(EInit) :
			mName("")
		{

		}

		void copyFrom(const RHIUniformBufferLayout& src)
		{
			mConstantBufferSize = src.mConstantBufferSize;
			mResourceOffset = src.mResourceOffset;
			mResource = src.mResource;
			mName = src.mName;
			mHashs = src.mHashs;
			mBComputedHash = src.mBComputedHash;
		}
		const std::string getDebugName() const { return mName; }
	private:
		std::string mName;
		mutable size_t mHashs{ 0 };
		mutable bool mBComputedHash{ false };
	public:
		uint32_t mConstantBufferSize{ 0 };
		uint32_t mResourceOffset{ 0 };
		std::vector<uint8_t> mResource;
	};


	inline bool operator == (const RHIUniformBufferLayout & a, const RHIUniformBufferLayout & b)
	{
		return a.mConstantBufferSize == b.mConstantBufferSize
			&& a.mResourceOffset == b.mResourceOffset
			&& a.mResource == b.mResource;
	}

	class RHIUniformBuffer : public RHIResource
	{
		
	};

	
















	typedef RHIUniformBuffer*					UniformBufferRHIParamRef;
	typedef std::shared_ptr<RHIUniformBuffer>	UniformBufferRHIPtr;


	typedef RHISamplerState*					SamplerStateRHIParamRef;
	typedef std::shared_ptr<RHISamplerState>	SamplerStateRHIPtr;

	typedef RHIRasterizerState*					RasterizerStateRHIParamRef;
	typedef std::shared_ptr<RHIRasterizerState>	RasterizerStateRHIPtr;

	typedef RHIDepthStencilState*				DepthStencilStateRHIParamRef;
	typedef std::shared_ptr<RHIDepthStencilState>	DepthStencilStateRHIPtr;

	typedef RHIBlendState*						BlendStateRHIParamRef;
	typedef std::shared_ptr<RHIBlendState>		BlendStateRHIPtr;


}
#endif
