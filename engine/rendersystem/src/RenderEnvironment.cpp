
#include "Context.h"
#include "Engine.h"
#include "app/include/App3D.hpp"
#include "Camera.hpp"
#include "rendersystem/include/Light.hpp"
#include "rendersystem/include/RenderEnvironment.hpp"

namespace Air
{
	const size_t RenderEnvironment::RE_EVENT_UPDATE_DIRECTION_LIGHT = CT_HASH("updateDirectionLight");



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
	void RenderEnvironment::updateLights(std::vector<LightSourcePtr> const & lights)
	{
		auto b = mSharedConstanBuffers.find("cb_RenderEnvironment");
		if (b != mSharedConstanBuffers.end())
		{
			for (auto l : lights)
			{
				switch (l->getType())
				{
				case LT_Directional:
				{
					DirectLightSourcePtr light = std::static_pointer_cast<DirectLightSource>(l);
					*b->second->getParameterByName("u_DirLightDir") = light->getDirection();
					*b->second->getParameterByName("u_DirLightColor") = light->getColor();
					this->notify(RE_EVENT_UPDATE_DIRECTION_LIGHT, light.get());
				}

				break;

				case LT_Ambient:
				{
					AmbientLightSourcePtr light = std::static_pointer_cast<AmbientLightSource>(l);
					*b->second->getParameterByName("u_DiffuseSpecularMip") = light->getMipmapNum();
					*b->second->getParameterByName("u_AmbientCubemapColor") = light->getColor();
				}
				break;
				default:
					break;
				}
			}
		}
	}

	ShadowMapData& RenderEnvironment::getShadowMapData()
	{
		return mShadowMapData;
	}
	void RenderEnvironment::updateShadowData()
	{
		auto b = mSharedConstanBuffers.find("cb_RenderEnvironment");
		if (b != mSharedConstanBuffers.end())
		{
			*b->second->getParameterByName("u_ShadowMatrix") = mShadowMapData.mShadowMatrix;
			*b->second->getParameterByName("u_ViewDistanceClip") = mShadowMapData.mViewDistances;
		}
	}

	void RenderEnvironment::update()
	{
		auto b = mSharedConstanBuffers.find("cb_RenderEnvironment");
		if (b != mSharedConstanBuffers.end())
		{
			SharedConstantBuffer* cb = b->second.get();
			*cb->getParameterByName("u_CameraDir") = Engine::getInstance().getAppInstance().getActiveCamera().getForwardVec();

			if (mCamera)
			{
				*cb->getParameterByName("u_ViewProjMatrix") = mCamera->getViewProjMatrixWOAdjust();

				*cb->getParameterByName("u_ViewProjMatrixInv") = mCamera->getInverseViewProjMatrixWOAdjust();

				*cb->getParameterByName("u_ViewMatrix") = mCamera->getViewMatrix();
			}
			
		}
	}

	void RenderEnvironment::setCamera(Camera & camera)
	{
		//
		mCamera = &camera;
	}
}