#include "Engine.h"
#include "rendersystem/include/RenderEnvironment.hpp"

namespace Air
{
	RenderEnvironment::RenderEnvironment()
	{

	}
	SharedConstantBuffer* const RenderEnvironment::getConstantBuffer(std::string name) const
	{
		auto item = mSharedConstanBuffers.find(name);
		return item == mSharedConstanBuffers.end() ? nullptr : item->second.get();
	}

	SharedConstantBuffer* RenderEnvironment::addConstantBuffer(std::string name)
	{
		auto cb = mSharedConstanBuffers.find(name);
		if (cb == mSharedConstanBuffers.end())
		{
			mSharedConstanBuffers.emplace(name, MakeUniquePtr<SharedConstantBuffer>());
			return mSharedConstanBuffers.find(name)->second.get();
		}
		return cb->second.get();
	}

	void RenderEnvironment::update()
	{
		auto b = mSharedConstanBuffers.find("color_buffer");
		if (b != mSharedConstanBuffers.end())
		{
			
		}
	}

}