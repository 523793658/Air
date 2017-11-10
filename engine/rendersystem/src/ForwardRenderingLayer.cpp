
#include "Context.h"
#include "SingletonManager.hpp"
#include "app/include/App3D.hpp"
#include "Camera.hpp"
#include "ConfigTranslator.hpp"
#include "basic/include/XMLDom.hpp"
#include "scene_manager/include/SceneManager.hpp"
#include "scene_manager/include/SceneObject.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderLayerPass.hpp"
#include "rendersystem/include/ShadowMap.hpp"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/Renderable.hpp"
#include "rendersystem/include/Viewport.hpp"
#include "rendersystem/include/RenderPipeline.hpp"
#include "rendersystem/include/ForwardRenderingLayer.hpp"

namespace Air
{
	class HelperPass : public RenderLayerPass
	{
	protected:
		void prepareObject()
		{
			SingletonManager::getSceneManagerInstance().querySceneObject(*mRenderTarget->getViewport()->mCamera, SceneObject::SOA_Helper, mSceneObjects);
		}
	};

	class ForwardLightingPass : public RenderLayerPass
	{
	protected:
		void prepareObject()
		{
			
			SingletonManager::getSceneManagerInstance().querySceneObject(*mRenderTarget->getViewport()->mCamera, SceneObject::SOA_Cullable, mSceneObjects);
		}
		void prepareRenderQueue()
		{
			for (auto obj : mSceneObjects)
			{
				if (0 == obj->getNumChildren())
				{
					auto renderable = obj->getRenderable().get();

					if (renderable && (renderable->isOpaque()))
					{
						renderable->setPass(PT_ForwardRendering);
						if (0 == renderable->getNumInstances())
						{
							addToRenderQueue(renderable);
						}
						renderable->addInstance(obj);
					}
				}
			}
		}
	};

	class SkyBoxPass : public RenderLayerPass
	{
	protected:
		void prepareObject() override
		{
			SingletonManager::getSceneManagerInstance().getNoCullableObject(mSceneObjects);
		}
	};

	class DirectionalLightShadowPass : public RenderLayerPass
	{
	public:
		void setReferenceCamera(CameraPtr const & camera)
		{
			mReferenceCamera = camera;
		}

		void setShadowMapNum(uint32_t num)
		{
			mNumShadowMap = num;
		}

		void initialize() override
		{
			RenderFactory& rf = SingletonManager::getRenderFactoryInstance();
			mShadowMaps = rf.MakeTexture2D(512, 512, 1, mNumShadowMap, EF_D16, 1, 0, EAH_GPU_Read | EAH_GPU_Write);
			mFrameBuffers.resize(mNumShadowMap);
			for (int i = 0; i < mNumShadowMap; ++i)
			{
				mFrameBuffers[i] = rf.makeFrameBuffer();
				RenderViewPtr view = rf.Make2DDepthStencilRenderView(mShadowMaps, i, 1, 0);
				mFrameBuffers[i]->attach(FrameBuffer::ATT_DepthStencil, view);
				
			}
		}

		virtual void doPass() override
		{
			RenderEngine& re = SingletonManager::getRenderFactoryInstance().getRenderEngineInstance();
			for (int index = 0; index < mNumShadowMap; ++index)
			{
				ShadowUtil::adjustShadowCamera(mReferenceCamera.get(), mFrameBuffers[index]->getViewport()->mCamera.get(), float3(0, 0, 1), static_cast<float>(index) / mNumShadowMap, (index + 1.0f) / mNumShadowMap);

				mRenderTarget = mFrameBuffers[index];
				mSceneObjects.clear();
				prepareObject();
				prepareRenderQueue();
				flushQueue();
			}
		}

	protected:
		void prepareObject()
		{
			SingletonManager::getSceneManagerInstance().querySceneObject(*mRenderTarget->getViewport()->mCamera, SceneObject::SOA_Cullable, mSceneObjects);
		}

		void prepareRenderQueue()
		{
			for (auto obj : mSceneObjects)
			{
				if (0 == obj->getNumChildren())
				{
					auto renderable = obj->getRenderable().get();
					if (0 == renderable->getNumInstances())
					{
						renderable->setPass(PT_GenShadowMap);
						addToRenderQueue(renderable);
					}
					renderable->addInstance(obj);
				}
			}
		}

	private:
		CameraPtr mReferenceCamera;
		uint32_t mNumShadowMap{ 3 };
		TexturePtr mShadowMaps;
		std::vector<FrameBufferPtr> mFrameBuffers;
		float3 mLightDirection{ 1, 1, 1 };
	};

	ForwardRenderingLayer::ForwardRenderingLayer(RenderPipeline* pipeline)
		:RenderLayer(pipeline)
	{
		mEnv = &SingletonManager::getRenderFactoryInstance().getRenderEngineInstance().getRenderEnvironment();
	}


	void ForwardRenderingLayer::beforeUpdate()
	{
		mLightObjects.clear();
		CameraPtr camera = std::static_pointer_cast<Camera>(mPipeline->getGlobalResource("mainCamera"));
		SingletonManager::getSceneManagerInstance().querySceneObject(*camera, SceneObject::SOA_LightSource, mLightObjects);
		mLights.clear();
		for (auto obj : mLightObjects)
		{
			mLights.push_back(std::static_pointer_cast<LightSource>(obj->getCustomData()));
		}
		mEnv->updateLights(mLights);
	}

	void ForwardRenderingLayer::endUpdate()
	{

	}

	void ForwardRenderingLayer::loadFromXml(XMLNodePtr node)
	{
		XMLNodePtr passNode = node->getFirstNode("pass");
		while (passNode)
		{
			RenderLayerPassType type = getRLPTFromStr(passNode->getAttribString("type"));
			switch (type)
			{
			case Air::RLPT_ShadowMap:
			{
				std::shared_ptr<DirectionalLightShadowPass> pass = MakeSharedPtr<DirectionalLightShadowPass>();
				mPasses.emplace_back(pass);
				
				pass->setReferenceCamera(std::static_pointer_cast<Camera>(mPipeline->getGlobalResource(passNode->getAttribString("referenceCamera", "mainCamera"))));
			}
				break;

			case Air::RLPT_ForwardRendering:
				mPasses.emplace_back(MakeSharedPtr<ForwardLightingPass>());
				break;

			case Air::RLPT_SkyBox:
				mPasses.emplace_back(MakeSharedPtr<SkyBoxPass>());
				break;

			case Air::RLPT_Helper:
				mPasses.emplace_back(MakeSharedPtr<HelperPass>());
				break;
			default:
				break;
			}
			std::string renderTargetName = passNode->getAttribString("renderTarget");
			if (renderTargetName != "")
			{
				FrameBufferPtr const & target = std::static_pointer_cast<FrameBuffer>(mPipeline->getGlobalResource(renderTargetName));
				mPasses.back()->setRenderTarget(target);
			}
			mPasses.back()->initialize();
			passNode = passNode->getNextSibling("pass");
		}
	}

}