#include "Context.h"
#include "SingletonManager.hpp"
#include "ResLoader.h"
#include "ConfigTranslator.hpp"
#include "basic/include/XMLDom.hpp"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/RenderLayer.hpp"
#include "rendersystem/include/PostProcess.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderView.hpp"
#include "rendersystem/include/Viewport.hpp"
#include "rendersystem/include/ForwardRenderingLayer.hpp"
#include "rendersystem/include/RenderPipeline.hpp"

namespace Air
{
	enum LayerType
	{
		LT_ForwardRendering,
		LT_DeferredRendering,
		LT_Overlay
	};

	RenderPipeline::RenderPipeline()
	{
		mPostProcessChain = MakeSharedPtr<PostProcessChain>();


	}

	RenderEnvironment* RenderPipeline::getRenderEnvironment()
	{
		return mRenderEnvironment;
	}


	void RenderPipeline::setFrameBuffer(FrameBufferPtr const & fb)
	{
		mScreenFrameBuffer = fb;
		auto it = mGlobalResource.find("finalTarget");
		if (it == mGlobalResource.end())
		{
			mGlobalResource.emplace("finalTarget", fb);
		}
		else
		{
			it->second = fb;
		}
		RenderViewPtr depthView = mScreenFrameBuffer->getAttached(FrameBuffer::ATT_DepthStencil);
		if (depthView)
		{
			mGlobalResource.emplace("sceneDepthView", depthView);
			mGlobalResource.emplace("sceneDepthTex", depthView->getSrcTexture());
		}
		mGlobalResource.emplace("mainCamera", mScreenFrameBuffer->getViewport()->mCamera);
	}

	void RenderPipeline::setConfig()
	{

	}
	void RenderPipeline::resize()
	{

	}
	void RenderPipeline::addGlobalResource(std::string const & name, std::shared_ptr<void> resouce)
	{
		auto it = mGlobalResource.find(name);
		if (it == mGlobalResource.end())
		{
			mGlobalResource.emplace(name, resouce);
		}
		else
		{
			it->second = resouce;
		}
	}
	std::shared_ptr<void> RenderPipeline::removeGloobalResource(std::string const & name)
	{
		auto it = mGlobalResource.find(name);
		if (it != mGlobalResource.end())
		{
			mGlobalResource.erase(it);
			return it->second;
		}
		return std::shared_ptr<void>();
	}

	std::shared_ptr<void> RenderPipeline::getGlobalResource(std::string const & name)
	{
		auto it = mGlobalResource.find(name);
		if (it != mGlobalResource.end())
		{
			return it->second;
		}
		return std::shared_ptr<void>();
	}

	void RenderPipeline::loadConfig(std::string path)
	{
		XMLDocument doc;
		XMLNodePtr root = doc.parse(ResLoader::getInstance().open(path));  
		XMLNodePtr frameBufferNode = root->getFirstNode("frameBuffer");
		RenderFactory& rf = SingletonManager::getRenderFactoryInstance();
		while (frameBufferNode)
		{
			FrameBufferPtr buffer = rf.makeFrameBuffer();
			uint32_t width, height;
			std::string targetName = frameBufferNode->getAttribString("name");
			buffer->setDebugInfo(targetName);

			XMLAttributePtr const & cameraAttr = frameBufferNode->getAttrib("camera");
			if (cameraAttr)
			{
				std::string cameraName = cameraAttr->getValueString();
				 auto camera = getGlobalResource(cameraName);
				 if (camera)
				 {
					 buffer->getViewport()->mCamera = std::static_pointer_cast<Camera>(camera);
				 }
				 else
				 {
					 addGlobalResource(cameraName, buffer->getViewport()->mCamera);
				 }
			}

			if (frameBufferNode->getAttribString("sizeMode") == "rel")
			{
				width = mScreenFrameBuffer->getWidth() * frameBufferNode->getAttribFloat("width", 1.0);
				height = mScreenFrameBuffer->getHeight() * frameBufferNode->getAttribFloat("height", 1.0);
			}
			else
			{
				width = frameBufferNode->getAttribUInt("width");
				height = frameBufferNode->getAttribUInt("height");
			}

			XMLAttributePtr clearMaskNode = frameBufferNode->getAttrib("clearMask");
			if (clearMaskNode)
			{
				buffer->setClearFlag(true, clearMaskNode->getValueUInt());
			}


			XMLNodePtr viewNode = frameBufferNode->getFirstNode("renderView");
			while (viewNode)
			{
				std::string viewName = viewNode->getAttribString("name");
				FrameBuffer::Attachment attach = getAttachmentFromStr(viewNode->getAttribString("attachment"));
				auto viewPtr = mGlobalResource.find(viewName);
				if (viewPtr != mGlobalResource.end())
				{
					buffer->attach(attach, std::static_pointer_cast<RenderView>(viewPtr->second));
				}
				else
				{
					ElementFormat format = parseFormat(viewNode->getAttribString("format"));
					int arraysize = viewNode->getAttribInt("arraySize", 1);
					TexturePtr tex = rf.MakeTexture2D(width, height, 1, arraysize, format, 1, 0, EAH_GPU_Write | EAH_GPU_Read);
					std::string texName = viewNode->getAttribString("texName");
					RenderViewPtr renderView;
					if (attach == FrameBuffer::ATT_DepthStencil)
					{
						renderView = rf.Make2DDepthStencilRenderView(tex,0, arraysize, 0);
					}
					else
					{
						renderView = rf.Make2DRenderView(tex, 0, 0, 0);
					}
					addGlobalResource(texName, tex);
					addGlobalResource(viewName, renderView);
					buffer->attach(attach, renderView);
				}
				viewNode = viewNode->getNextSibling("renderView");
			}
			mFrameBuffers.push_back(buffer);

			addGlobalResource(targetName, buffer);
			frameBufferNode = frameBufferNode->getNextSibling("frameBuffer");
		}







		XMLNodePtr layerNode = root->getFirstNode("layer");
		while (layerNode)
		{
			LayerType type = static_cast<LayerType>(layerNode->getAttribUInt("type", 0));
			RenderLayer* layer = nullptr;
			switch (type)
			{
			case Air::LT_ForwardRendering:
				layer = new ForwardRenderingLayer(this);
				break;
			case Air::LT_DeferredRendering:
			case Air::LT_Overlay:
			default:
				AIR_UNREACHABLE("invalid render layer type");
				break;
			}
			layer->loadFromXml(layerNode);
			mRenderLayers.push_back(layer);
			layerNode = layerNode->getNextSibling("layer");
		}

		XMLNodePtr postProccessNode = root->getFirstNode("postProcessChain");
		std::string inputName = postProccessNode->getAttribString("input");
		std::string ouputName = postProccessNode->getAttribString("output");

		mPostProcessChain->setInputTarget(std::static_pointer_cast<FrameBuffer>(getGlobalResource(inputName)));
		mPostProcessChain->setOutputTarget(std::static_pointer_cast<FrameBuffer>(getGlobalResource(ouputName)));
		mPostProcessChain->loadCfg(postProccessNode);
	}

	void RenderPipeline::update()
	{
		static const Color clearColor(1, 0, 0, 1);
		for (auto target : mFrameBuffers)
		{
			target->clear(FrameBuffer::CBM_Color | FrameBuffer::CBM_Depth, clearColor, 1.0, 0);
		}

		for (auto layer : mRenderLayers)
		{
			layer->update();
		}

		postProcess(false);
	}

	void RenderPipeline::postProcess(bool skip)
	{
		if (mPostProcessChain)
		{
			mPostProcessChain->update();
		}
	}

	void RenderPipeline::rebuild()
	{
		mPostProcessChain = MakeSharedPtr<PostProcessChain>();
		mPostProcessChain->loadCfg(mConfig.mPostProcessConfigPath);
	}
}