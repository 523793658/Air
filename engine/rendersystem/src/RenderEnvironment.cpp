#include "Engine.h"
#include "rendersystem/include/RenderEnvironment.hpp"

namespace Air
{
	RenderEnvironment::RenderEnvironment()
	{

	}

	RenderEffectConstantBufferPtr const RenderEnvironment::getConstantBuffer(std::string name) const
	{
		auto item = mSharedConstanBuffers.find(name);
		return item == mSharedConstanBuffers.end() ? RenderEffectConstantBufferPtr() : item->second;
	}

	void RenderEnvironment::addConstantBuffer(std::string name, RenderEffectConstantBufferPtr cbuffer)
	{
		if (mSharedConstanBuffers.find(name) == mSharedConstanBuffers.end())
		{
			mSharedConstanBuffers.emplace(name, cbuffer);
		}
	}

}