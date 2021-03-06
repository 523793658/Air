
#include "Context.h"
#include "SingletonManager.hpp"
#include "core/include/ResLoader.h"
#include "basic/include/XMLDom.hpp"
#include "rendersystem/include/SimpleMeshFactory.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderEngine.hpp"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/Viewport.hpp"
#include "rendersystem/include/RenderView.hpp"
#include "rendersystem/include/ToneMapping.hpp"
#include "rendersystem/include/Mesh.hpp"
#include "rendersystem/include/PostProcess.hpp"



namespace Air
{
	std::unordered_map<std::string, PostProcesserCreator*> PostProcessChain::mProcesserCreators;

	PostProcesser::PostProcesser()
	{

	}

	PostProcesser::~PostProcesser()
	{

	}

	void PostProcesser::setOutputFrameBuffer(FrameBufferPtr const & output)
	{
		mOutputFrameBuffer = output;
	}

	void PostProcesser::setOuputTexture(uint16_t index, TexturePtr const & tex)
	{
		if (mOutputTexture.size() <= index)
		{
			mOutputTexture.resize(index + 1);
		}
		mOutputTexture[index] = tex;
		mDirty = true;
	}

	void PostProcesser::setConfig(PostProcessConfigPtr config)
	{
		mConfig = config;
		mDirty = true;
	}

	void PostProcesser::setInputTexture(uint16_t index, TexturePtr srcTex)
	{
		if (index >= mSrcTextures.size())
		{
			mSrcTextures.resize(index + 1);
		}
		mSrcTextures[index] = srcTex;
		mDirty = true;
	}

	void PostProcesser::onRenderBegin()
	{

	}
	void PostProcesser::onRenderEnd()
	{

	}

	void PostProcessChain::setInputTarget(FrameBufferPtr const & fb)
	{
		mInputframeBuffer = fb;
	}

	void PostProcessChain::setOutputTarget(FrameBufferPtr const & fb)
	{
		mOutputFrameBuffer = fb;
	}


	PostProcessChain::PostProcessChain()
	{
		mRenderable = SimpleMeshFactory::createStaticQuat(1.f);
	}

	void PostProcessChain::loadCfg(XMLNodePtr root)
	{
		std::string format = root->getAttribString("colorFormat", "EF_ABGR8");
		mColorFormat = parseFormat(format);


		XMLNodePtr processerNode = root->getFirstNode("postProcesser");
		while (processerNode)
		{
			std::string name = processerNode->getAttribString("name");
			PostProcessConfigPtr config = PostProcessChain::mProcesserCreators.find(name)->second->loadCfg(processerNode);
			config->mChain = this;
			config->mName = name;
			mConfigs.push_back(config);

			processerNode = processerNode->getNextSibling("postProcesser");
		}
	}

	void PostProcessChain::loadCfg(std::string cfgPath)
	{
		XMLDocument doc;
		XMLNodePtr root = doc.parse(ResLoader::getInstance().open(cfgPath));
		loadCfg(root);
	}

	void PostProcessChain::assemblePostProcessChain()
	{



		RenderFactory& rf = SingletonManager::getRenderFactoryInstance();
		RenderEngine& engine = rf.getRenderEngineInstance();
		RenderDeviceCaps const & caps = engine.getDeviceCaps();
		FrameBufferPtr lastOutput;
		uint32_t width, height;
		for (auto it : mPostProcessers)
		{
			delete it;
		}

		width = mInputframeBuffer->getWidth();
		height = mInputframeBuffer->getHeight();
		ElementFormat fmt = mInputframeBuffer->getAttached(FrameBuffer::ATT_Color0)->getFormat();
		lastOutput = mInputframeBuffer;
		for (size_t i = 0; i < mConfigs.size(); ++i)
		{
			PostProcessConfigPtr const & config = mConfigs[i];
			PostProcesser* processer = createPostProcesser(config);
			Texture* colorTex = lastOutput->getAttached(FrameBuffer::ATT_Color0)->getSrcTexture().get();

			FrameBufferPtr output;
			if (i == mConfigs.size() - 1)
			{
				output = mOutputFrameBuffer;
			}
			else
			{
				if (mTempFrameBuffers.size() > 0)
				{
					output = mTempFrameBuffers.top();
					mTempFrameBuffers.pop();
				}
				else
				{
					output = rf.makeFrameBuffer();
					TexturePtr ct = rf.MakeTexture2D(width, height, 1, 1, fmt, 1, 0, EAH_GPU_Read | EAH_GPU_Write);
					output->attach(FrameBuffer::ATT_Color0, rf.Make2DRenderView(ct, 0, 1, 0));
				}
			}
			processer->setInputTexture(0, colorTex->shared_from_this());
			processer->setOutputFrameBuffer(output);
			mPostProcessers.push_back(processer);
			mTempFrameBuffers.push(lastOutput);
			lastOutput = output;
		}
	}


	void PostProcessChain::update()
	{
		if (mIsDirty)
		{
			this->assemblePostProcessChain();
			mIsDirty = false;
		}
		for (auto processer : mPostProcessers)
		{
			processer->update();
		}

		for (auto processer : mPostProcessers)
		{
			processer->render();
		}

	}

	PostProcesser* PostProcessChain::createPostProcesser(PostProcessConfigPtr config)
	{
		return mProcesserCreators.find(config->mName)->second->createInstance(config);
	}

	void PostProcessChain::registerProcesser(std::string name, PostProcesserCreator* creater)
	{
		if (mProcesserCreators.find(name) == mProcesserCreators.end())
		{
			mProcesserCreators.emplace(name, creater);
		}
	}

	PostProcesserCreator* PostProcessChain::getProcesserCreator(std::string name)
	{
		BOOST_ASSERT(mProcesserCreators.find(name) != mProcesserCreators.end());
		return mProcesserCreators.find(name)->second;
	}

	RenderablePtr const & PostProcessChain::getRenderable() const
	{
		return mRenderable;
	}
}