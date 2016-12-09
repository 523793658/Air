#include "Engine.h"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderEngine.hpp"
#include "app/include/App3D.hpp"
#include "rendersystem/include/FrameBuffer.hpp"
#include "scene_manager/include/SceneObject.hpp"
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
// 		std::lock_guard<std::mutex> lock(mUpdateMutex);
// 		mURT = urt;
// 		RenderEngine& re = Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
// 		App3DFramework& app = Context::getInstance().getAppInstance();
// 		float const app_time = app.getAppTime();
// 		float const frame_time = app.getFrameTime();
// 
// 		mNumObjectsRendered = 0;
// 		mNumRenderablesRendered = 0;
// 		mNumPrimitivesRendered = 0;
// 		mNumVerticesRendered = 0;
// 		Camera& camera = app.getActiveCamera();
// 		auto const & scene_objs = (urt & App3DFramework::URV_Overlay) ? mOverlaySceneObjs : mSceneObjs;
// 		for (auto const & scene_obj : scene_objs)
// 		{
// 			scene_obj->setVisibleMark(BO_No);
// 		}
// 		if (urt & App3DFramework::URV_NeedFlush)
// 		{
// 
// 		}
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
				obj->update
			}
		}
	}
}