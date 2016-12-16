#include "Engine.h"
#include "Camera.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderEngine.hpp"
#include "app/include/App3D.hpp"
#include "rendersystem/include/FrameBuffer.hpp"
#include "scene_manager/include/SceneObject.hpp"
#include "boost/functional/hash.hpp"

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

	void SceneManager::update()
	{
		RenderEngine& renderEngine = Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		renderEngine.beginFrame();
		this->flushScene();
	}

	void SceneManager::flush(uint32_t urt)
	{
		std::lock_guard<std::mutex> lock(mUpdateMutex);
		mURT = urt;
		RenderEngine& re = Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		App3DFramework& app = Context::getInstance().getAppInstance();
		float const app_time = app.getAppTime();
		float const frame_time = app.getFrameTime();

		mNumObjectsRendered = 0;
		mNumRenderablesRendered = 0;
		mNumPrimitivesRendered = 0;
		mNumVerticesRendered = 0;
		Camera& camera = app.getActiveCamera();
		auto const & scene_objs = (urt & App3DFramework::URV_Overlay) ? mOverlaySceneObjs : mSceneObjs;
		for (auto const & scene_obj : scene_objs)
		{
			scene_obj->setVisibleMark(BO_No);
		}
		if (urt & App3DFramework::URV_NeedFlush)
		{
			mFrustum = &camera.getViewFrustum();
			std::vector<uint32_t> visible_list((mSceneObjs.size() + 31) / 32, 0);
			for (size_t i = 0; i < mSceneObjs.size(); ++i)
			{
				if (mSceneObjs[i]->isVisible())
				{
					visible_list[i / 32] != (1UL << (i & 31));
				}
			}
			size_t seed = 0;
			boost::hash_range(seed, visible_list.begin(), visible_list.end());
			boost::hash_combine(seed, camera.getOmniDirectionalMode());
			boost::hash_combine(seed, &camera);

			auto vmiter = mVisibleMarksMap.find(seed);
			if (vmiter == mVisibleMarksMap.end())
			{
				this->clipScene();
			}
		}
	}

	BoundOverlap SceneManager::visibleTestFromParent(SceneObject* obj, float3 const & eye_pos, float4x4 const & view_proj)
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
					visible = AABB::Intersects()
				}
			}
		}
	}

	void SceneManager::clipScene()
	{
		App3DFramework& app = Context::getInstance().getAppInstance();
		Camera& camera = app.getActiveCamera();
		float4x4 viewProjMat = camera.getViewProjMatrix();
		for (auto const & obj : mSceneObjs)
		{
			auto so = obj.get();
			BoundOverlap visible;
			uint32_t const attr = so->getAttrib();
			if (so->isVisible());
			{
				visible = this->Visible
			}
		}
	}

	void SceneManager::flushScene()
	{
		RenderEngine& re = Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		mVisibleMarksMap.clear();
		uint32_t urt;
		App3DFramework& app = Context::getInstance().getAppInstance();
		for (uint32_t pass = 0; ; ++pass)
		{
			re.beginPass();
			urt = app.update(pass);
			if (urt & App3DFramework::URV_NeedFlush)
			{
				this->flush(urt);
			}
			re.endPass();
			if (urt & App3DFramework::URV_Finished)
			{
				break;
			}
		}
// 		re.postProcess((urt & App3DFramework::URV_SkipPostProcess) != 0);
// 		if (re.getStereo() != STM_None)
// 		{
// 			re.bindFrameBuffer(re.getOverlayFrameBuffer());
// 			re.getCurrentFrameBuffer()->clear(FrameBuffer::CBM_Color | FrameBuffer::CBM_Depth, Color(0, 0, 0, 0), 1.0f, 0);
// 		}
// 		this->flush(App3DFramework::URV_Overlay);
// 		//re.ste
		mNumDrawCalls = re.getNumDrawsJustCalled();
		mNumDispatchCalls = re.getNumDispatchesJustCalled();
	}

	void SceneManager::addSceneObject(SceneObjectPtr const & obj)
	{
		std::lock_guard<std::mutex> lock(mUpdateMutex);
		this->addSceneObjectLocked(obj);
	}

	void SceneManager::addSceneObjectLocked(SceneObjectPtr const & obj)
	{
		App3DFramework& app = Context::getInstance().getAppInstance();
		float const app_time = app.getAppTime();
		float const frame_time = app.getFrameTime();

		//todo 更新渲染线程数据
		obj->mainThreadUpdate(app_time, frame_time);

		uint32_t const attr = obj->getAttrib();
		if (attr & SceneObject::SOA_Overlay)
		{
			mOverlaySceneObjs.push_back(obj);
		}
		else
		{
			if ((attr & SceneObject::SOA_Cullable) && !(attr & SceneObject::SOA_Moveable))
			{
				obj->updateWorldMatrix();
			}
			mSceneObjs.push_back(obj);
			this->onAddSceneObject(obj);
		}
	}
}