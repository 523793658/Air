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
					visible_list[i / 32] |= (1UL << (i & 31));
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
				std::shared_ptr<std::vector<BoundOverlap>> visible_marks
					= MakeSharedPtr<std::vector<BoundOverlap>>(scene_objs.size());
				for (size_t i = 0; i < scene_objs.size(); ++i)
				{
					(*visible_marks)[i] = scene_objs[i]->getVisibleMark();
				}
				mVisibleMarksMap.emplace(seed, visible_marks);
			}
			else
			{
				for (size_t i = 0; i < scene_objs.size(); ++i)
				{
					scene_objs[i]->setVisibleMark((*vmiter->second)[i]);
				}
			}
		}
		if (urt & App3DFramework::URV_Overlay)
		{
			for (auto const & scene_obj : scene_objs)
			{
				scene_obj->mainThreadUpdate(app_time, frame_time);
				scene_obj->setVisibleMark(scene_obj->isVisible() ? BO_Yes : BO_No);
			}
		}
		std::vector<std::pair<Renderable*, std::vector<SceneObject*>>> renderables;
		{
			std::map<Renderable*, size_t> renderables_map;
			for (auto const & obj : scene_objs)
			{
				auto so = obj.get();
				if ((so->getVisibleMark() != BO_No) && (0 == so->getNumChildren()))
				{
					auto renderable = so->getRenderable().get();
					if (renderable)
					{
						auto iter = renderables_map.lower_bound(renderable);
						if ((iter != renderables_map.end()) && (iter->first == renderable))
						{
							renderables[iter->second].second.push_back(so);
						}
						else
						{
							renderables_map.emplace(renderable, renderables.size());
							renderables.emplace_back(renderable, std::vector<SceneObject*>(1, so));
						}
						++mNumObjectsRendered;
					}
				}
			}
		}
		for (auto const & renderable : renderables)
		{
			Renderable& ra(*renderable.first);
			ra.assignInstances(renderable.second.begin(), renderable.second.end());
			ra.addToRenderQueue();
		}
		std::sort(mRenderQueue.begin(), mRenderQueue.end(), [](std::pair<RenderTechnique const *, std::vector<Renderable*>> const & lhs,
			std::pair<RenderTechnique const *, std::vector<Renderable*>> const & rhs)
		{
			BOOST_ASSERT(lhs.first);
			BOOST_ASSERT(rhs.first);
			return lhs.first->getWeight() < rhs.first->getWeight();
		});
		float4 const & view_mat_z = camera.getViewMatrix().col(2);
		for (auto & items : mRenderQueue)
		{
			for (auto const & item : items.second)
			{
				item->render();
			}
			mNumRenderablesRendered += static_cast<uint32_t>(items.second.size());
		}
		mRenderQueue.resize(0);
		mNumPrimitivesRendered += re.getNumPrimitivesJustRendered();
		mNumVerticesRendered += re.getNumVerticesJustRendered();
		urt = 0;
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
					visible = MathLib::perspective_area(eye_pos, view_proj, obj->getAABB()) > mSmallObjThreshold ? parent_bo : BO_No;
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
			if (so->isVisible())
			{
				visible = this->visibleTestFromParent(so, camera.getEyePos(), viewProjMat);
				if (BO_Partial == visible)
				{
					if (attr & SceneObject::SOA_Moveable) 
					{
						so->updateWorldMatrix();
					}
					if (attr & SceneObject::SOA_Cullable)
					{
						visible = (MathLib::perspective_area(camera.getEyePos(), viewProjMat, so->getAABB()) > mSmallObjThreshold) ? BO_Yes : BO_No;
					}
					else
					{
						visible = BO_Yes;
					}
					if (!camera.getOmniDirectionalMode() && (attr & SceneObject::SOA_Cullable) && (BO_Yes == visible))
					{
						visible = this->testAABBVisible(so->getAABB());
					}
				}
			}
			else
			{
				visible = BO_No;
			}
			so->setVisibleMark(visible);
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