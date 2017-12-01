#include "Context.h"
#include "SingletonManager.hpp"
#include "Camera.hpp"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/Viewport.hpp"
#include "scene_manager/include/SceneManager.hpp"
#include "scene_manager/include/SceneObject.hpp"
#include "rendersystem/include/RenderLayerPass.hpp"
#include "rendersystem/include/DeferredLightingLayer.hpp"
namespace Air
{
	class GBufferPass : public RenderLayerPass
	{
		void prepareObject() override
		{
			SingletonManager::getSceneManagerInstance().querySceneObject(*mRenderTarget->getViewport()->mCamera, SceneObject::SOA_Cullable, mSceneObjects);



		}
	};


	DeferredLightingLayer::DeferredLightingLayer(RenderPipeline* pipeline)
		:RenderLayer(pipeline)
	{
		
	}

	void DeferredLightingLayer::loadFromXml(XMLNodePtr node)
	{

	}

	void DeferredLightingLayer::beforeUpdate()
	{

	}

	void DeferredLightingLayer::endUpdate()
	{

	}
}