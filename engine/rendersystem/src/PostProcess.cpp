#include "Engine.h"
#include "rendersystem/include/SimpleMeshFactory.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderEngine.hpp"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/Viewport.hpp"
#include "rendersystem/include/PostProcess.hpp"


namespace Air
{
	PostProcessChain::PostProcessChain()
	{
		mRenderable = SimpleMeshFactory::createStaticQuat(1.0f);
	}

	void PostProcessChain::assemblePostProcessChain()
	{
		RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();
		RenderEngine& engine = rf.getRenderEngineInstance();
		mSceneFrameBuffer = rf.makeFrameBuffer();
		mSceneFrameBuffer->getViewport()->mCamera = engine.getScreenFrameBuffer()->getViewport()->mCamera;
	}


	void PostProcessChain::update()
	{
		if (mIsDirty)
		{
			this->assemblePostProcessChain();
			mIsDirty = false;
		}

	}
}