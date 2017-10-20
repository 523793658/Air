#include "Engine.h"
#include "app/include/App3D.hpp"
#include "Camera.hpp"
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

	SharedConstantBuffer* RenderEnvironment::addConstantBuffer(std::string name, ConstantBufferType type)
	{
		auto cb = mSharedConstanBuffers.find(name);
		if (cb == mSharedConstanBuffers.end())
		{
			mSharedConstanBuffers.emplace(name, MakeUniquePtr<SharedConstantBuffer>());
			return mSharedConstanBuffers.find(name)->second.get();
		}
		return cb->second.get();
		return nullptr;
	}

	void RenderEnvironment::update()
	{
		auto b = mSharedConstanBuffers.find("cb_RenderEnvironment");
		if (b != mSharedConstanBuffers.end())
		{
			*b->second->getParameterByName("u_DirLightDir") = float3(1, 2, 3).normalize();
			*b->second->getParameterByName("u_DirLightColor") = float3(1.0, 1.0, 1.0);

			*b->second->getParameterByName("u_CameraDir") = Engine::getInstance().getAppInstance().getActiveCamera().getForwardVec();

		}
	}

}