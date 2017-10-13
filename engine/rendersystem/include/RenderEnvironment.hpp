#ifndef _RenderEnvironment_H_
#define _RenderEnvironment_H_
#pragma once
#include "PreDeclare.h"
#include <unordered_map>
namespace Air
{
	class AIR_CORE_API RenderEnvironment : boost::noncopyable
	{
	public:
		RenderEnvironment();

		RenderEffectConstantBufferPtr const getConstantBuffer(std::string name) const;

		void addConstantBuffer(std::string name, RenderEffectConstantBufferPtr cbuffer);
	private:

		std::unordered_map<std::string, RenderEffectConstantBufferPtr> mSharedConstanBuffers;
		
	};



}


#endif
