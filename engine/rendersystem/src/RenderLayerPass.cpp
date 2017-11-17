#include "Context.h"
#include "SingletonManager.hpp"
#include "scene_manager/include/SceneObject.hpp"
#include "rendersystem/include/Renderable.hpp"
#include "rendersystem/include/Viewport.hpp"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "scene_manager/include/SceneManager.hpp"
#include "rendersystem/include/RenderLayerPass.hpp"

namespace Air
{
	void RenderLayerPass::initialize(RenderLayer* layer)
	{
		mLayer = layer;
	}

	void RenderLayerPass::doPass()
	{
		SingletonManager::getRenderFactoryInstance().getRenderEngineInstance().bindFrameBuffer(mRenderTarget);

		mSceneObjects.clear();
		prepareObject();
		prepareRenderQueue();
		flushQueue();
	}

	void RenderLayerPass::prepareRenderQueue()
	{
		for (auto obj : mSceneObjects)
		{
			if (0 == obj->getNumChildren())
			{
				auto renderable = obj->getRenderable().get();
				if (renderable)
				{
					if (0 == renderable->getNumInstances())
					{
						addToRenderQueue(renderable);
					}
					renderable->addInstance(obj);
				}
			}
		}
		mSceneObjects.clear();
	}

	void RenderLayerPass::addToRenderQueue(Renderable* obj)
	{
		if (obj->isHWResourceReady())
		{
			mRenderQueue.push_back(obj);
		}
	}

	void RenderLayerPass::flushQueue()
	{
		RenderEngine& re = SingletonManager::getRenderFactoryInstance().getRenderEngineInstance();
		re.bindFrameBuffer(mRenderTarget);
		int x = 0;
		for (auto renderable : mRenderQueue)
		{
			x++;
			renderable->render();
			renderable->clearInstance();
		}
		mRenderQueue.clear();
	}


	void RenderLayerPass::setRenderTarget(FrameBufferPtr const & target)
	{
		mRenderTarget = target;
	}

}