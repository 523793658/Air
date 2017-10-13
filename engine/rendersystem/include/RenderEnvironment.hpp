#ifndef _RenderEnvironment_H_
#define _RenderEnvironment_H_
#pragma once
#include "PreDeclare.h"
#include <unordered_map>
#include "rendersystem/include/RenderEffect.hpp"
namespace Air
{
	class AIR_CORE_API RenderEnvironment : boost::noncopyable
	{
	public:
		RenderEnvironment();

		SharedConstantBuffer* const getConstantBuffer(std::string name) const;

		SharedConstantBuffer* addConstantBuffer(std::string name, ConstantBufferType type);

		void update();
	private:

		std::unordered_map<std::string, std::unique_ptr<SharedConstantBuffer>> mSharedConstanBuffers;

		//std::vector<RenderEffectConstantBufferPtr> mConst
	};



}


#endif
