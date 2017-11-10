#include "Context.h"
#include "Engine.h"
#include "Camera.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderEngine.hpp"
#include "app/include/App3D.hpp"
#include "rendersystem/include/FrameBuffer.hpp"
#include "scene_manager/include/SceneObject.hpp"
#include "boost/functional/hash.hpp"
#include "rendersystem/include/Renderable.hpp"
#include "rendersystem/include/RenderEffect.hpp"
#include <map>

#include "scene_manager/include/SceneManager.hpp"

namespace Air
{
	SceneManager::SceneManager()
		:mDeferredMode(false), mNumDispatchCalls(0), mNumDrawCalls(0), mNumObjectsRendered(0),
		mNumPrimitivesRendered(0), mNumRenderablesRendered(0),
		mNumVerticesRendered(0), mUpdateElapse(1.0f / 60), mQuit(false), mSmallObjThreshold(0)
	{

	}
	SceneManager::~SceneManager()
	{

	}

	//更新节点等操作
	void SceneManager::update()
	{
		mVisibleMarksMap.clear();
	}


	BoundOverlap SceneManager::visibleTestFromParent(SceneObject* obj, float3 const & view_dir, float3 const & eye_pos, float4x4 const & view_proj)
	{
		BoundOverlap visible;
		if (obj->getParent())
		{
			BoundOverlap parent_bo = obj->getParent()->getVisibleMark();
			if (BO_No == parent_bo)
			{
				visible = BO_No;
			}
			else
			{
				uint32_t const attr = obj->getAttrib();
				if (attr & SceneObject::SOA_Moveable)
				{
					obj->updateWorldMatrix();
				}
				if (attr & SceneObject::SOA_Cullable)
				{
					if (mSmallObjThreshold > 0)
					{
						visible = ((MathLib::ortho_area(view_dir, obj->getAABB()) > mSmallObjThreshold) && (MathLib::perspective_area(eye_pos, view_proj, obj->getAABB()) > mSmallObjThreshold)) ? parent_bo : BO_No;
					}
					else
					{
						visible = parent_bo;
					}
				
				}
				else
				{
					visible = parent_bo;
				}
			}
		}
		else
		{
			visible = BO_Partial;
		}
		return visible;
	}

	void SceneManager::clipScene(uint32_t mask, std::vector<SceneObject*> & result)
	{
		App3DFramework& app = Engine::getInstance().getAppInstance();
		Camera& camera = app.getActiveCamera();
		float4x4 viewProjMat = camera.getViewProjMatrix();
		for (auto const & obj : mSceneObjs)
		{
			auto so = obj.get();
			BoundOverlap visible;
			uint32_t const attr = so->getAttrib();
			if (attr & mask)
			{
				if (SceneObject::SOA_Cullable & attr)
				{
					visible = this->visibleTestFromParent(so, camera.getForwardVec(), camera.getEyePos(), viewProjMat);
				
					if (BO_Partial == visible)
					{
						if (attr & SceneObject::SOA_Moveable)
						{
							so->updateWorldMatrix();
						}
						visible = (MathLib::perspective_area(camera.getEyePos(), viewProjMat, so->getAABB()) > mSmallObjThreshold) ? BO_Yes : BO_No;
						if (!camera.getOmniDirectionalMode() && (attr & SceneObject::SOA_Cullable) && (BO_Yes == visible))
						{
							visible = this->testAABBVisible(so->getAABB());
						}
					}
				}
				else
				{
					visible = BO_Yes;
				}
			}
			else
			{
				visible = BO_No;
			}
			if (visible != BO_No)
			{
				result.push_back(so);
			}
			so->setVisibleMark(visible);
		}
	}

	void SceneManager::addRenderable(Renderable* obj)
	{
		if (obj->isHWResourceReady())
		{
			bool add;
			if (obj->selectMode())
			{
				add = true;
			}
			else
			{
				if (mURT & App3DFramework::URV_OpaqueOnly)
				{
					add = !(obj->isTransparencyBackFace() || obj->isTransparencyFrontFace());
				}
				else if (mURT & App3DFramework::URV_TransparencyBackOnly)
				{
					add = obj->isTransparencyBackFace();
				}
				else if (mURT & App3DFramework::URV_TransparencyFrontOnly)
				{
					add = obj->isTransparencyFrontFace();
				}
				else
				{
					add = true;
				}

			}
			if (add)
			{
				RenderTechnique const * objTech = obj->getRenderTechnique();
				BOOST_ASSERT(objTech);
				bool found = false;
				for (auto & items : mRenderQueue)
				{
					if (items.first == objTech)
					{
						items.second.push_back(obj);
						found = true;
						break;
					}
				}
				if (!found)
				{
					mRenderQueue.emplace_back(objTech, std::vector<Renderable*>(1, obj));
				}
			}
		}
	}

	void SceneManager::addSceneObject(SceneObjectPtr const & obj)
	{
		std::lock_guard<std::mutex> lock(mUpdateMutex);
		this->addSceneObjectLocked(obj);
	}

	void SceneManager::addSceneObjectLocked(SceneObjectPtr const & obj)
	{
		App3DFramework& app = Engine::getInstance().getAppInstance();
		float const app_time = app.getAppTime();
		float const frame_time = app.getFrameTime();
		uint32_t const attr = obj->getAttrib();

		if (!(attr & SceneObject::SOA_Moveable))
		{
			obj->updateWorldMatrix();
		}
		//todo 更新渲染线程数据
		obj->mainThreadUpdate(app_time, frame_time);

		if (attr & SceneObject::SOA_Overlay)
		{
			mOverlaySceneObjs.push_back(obj);
		}
		else if(attr & SceneObject::SOA_Cullable)
		{
			
			mSceneObjs.push_back(obj);
			this->onAddSceneObject(obj);
		}
		else
		{
			mNoCullableSceneObjects.push_back(obj);
		}
	}

	void SceneManager::getNoCullableObject(std::vector<SceneObject*> & result)
	{
		for (auto it : mNoCullableSceneObjects)
		{
			result.push_back(it.get());
		}
	}


	void SceneManager::addLight(LightSourcePtr const & light)
	{
		std::lock_guard<std::mutex> lock(mUpdateMutex);
		this->addLightLocked(light);
	}
	void SceneManager::addLightLocked(LightSourcePtr const & light)
	{
		mLights.push_back(light);
	}

	void SceneManager::delLight(LightSourcePtr const & light)
	{
		auto it = std::find(mLights.begin(), mLights.end(), light);
		if (it != mLights.end())
		{
			mLights.erase(it);
		}
	}

	void SceneManager::querySceneObject(Camera const & camera, uint32_t mask, std::vector<SceneObject*> & result)
	{

		mFrustum = &camera.getViewFrustum();



		this->clipScene(mask, result);

		for (auto it : mNoCullableSceneObjects)
		{
			if (it->getAttrib() & mask)
			{
				result.push_back(it.get());
			}
		}
	}
}