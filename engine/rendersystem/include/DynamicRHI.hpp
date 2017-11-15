#ifndef _DynamicRHI_H_
#define _DynamicRHI_H_
#pragma once
#include "rendersystem/include/UniformBuffer.hpp"
#include "rendersystem/include/HardwareResource.hpp"
namespace Air
{

	class AIR_CORE_API DynamicRHI
	{
	public:
		virtual ~DynamicRHI(){}

		virtual void init() = 0;

		virtual void PostInit() {}

		virtual void shutdown() = 0;

		virtual const char* getName() = 0;

		//virtual SamplerStateRHIPtr createSamplerState(const Samplersate)
	};



	UniformBufferRHIPtr RHICreateUniformBuffer(const void * contents, const RHIUniformBufferLayout& layout, EUniformBufferUsage usage)
	{
		return UniformBufferRHIPtr();
	}
}
#endif
