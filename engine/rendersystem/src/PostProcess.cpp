#include "Engine.h"
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
	PostProcesser::PostProcesser()
		: mChain(nullptr)
	{

	}

	PostProcesser::PostProcesser(PostProcessChain* chain)
		:mChain(chain)
	{
	}

	PostProcesser::~PostProcesser()
	{

	}

	void PostProcesser::update()
	{
		RenderEngine& re = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		re.bindFrameBuffer(mOutputFrameBuffer);
	}

	void PostProcesser::render()
	{
	}

	void PostProcesser::setOutputFrameBuffer(FrameBufferPtr const & output)
	{
		mOutputFrameBuffer = output;
	}

	void PostProcesser::setInputTexture(uint16_t index, TexturePtr srcTex)
	{
		if (index >= mSrcTextures.size())
		{
			mSrcTextures.resize(index);
		}
		mSrcTextures[index] = srcTex;
	}

	PostProcessChain::PostProcessChain()
	{
		mRenderable = SimpleMeshFactory::createStaticQuat(1.0f);
	}

	void PostProcessChain::loadCfg(std::string cfgPath)
	{

	}

	void PostProcessChain::assemblePostProcessChain()
	{
		RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();
		RenderEngine& engine = rf.getRenderEngineInstance();
		RenderDeviceCaps const & caps = engine.getDeviceCaps();
		FrameBufferPtr lastOutput;
		FrameBufferPtr screenBuffer = engine.getScreenFrameBuffer();
		uint32_t width, height;
		width = screenBuffer->getWidth();
		height = screenBuffer->getHeight();

		mSceneFrameBuffer = rf.makeFrameBuffer();
		mSceneFrameBuffer->getViewport()->mCamera = engine.getScreenFrameBuffer()->getViewport()->mCamera;
		lastOutput = mSceneFrameBuffer;

		TexturePtr const & depthStencilTex = engine.getScreenDepthStencilTexture();


		RenderViewPtr dsView;
		dsView = rf.Make2DDepthStencilRenderView(*depthStencilTex, 0, 1, 0);

		ElementFormat fmt = EF_Unknown;
		if (caps.textureFormatSupport(mColorFormat) && caps.rendertargetFormatSupport(mColorFormat, 1, 0))
		{
			fmt = mColorFormat;
		}
		else if (caps.vertexFormatSupport(EF_ABGR8) && caps.rendertargetFormatSupport(EF_ABGR8, 1, 0))
		{
			fmt = EF_ABGR8;
		}
		else
		{
			BOOST_ASSERT(caps.textureFormatSupport(EF_ARGB8) && caps.rendertargetFormatSupport(EF_ARGB8, 1, 0));
			fmt = EF_ARGB8;
		}

		TexturePtr clr_tex = rf.MakeTexture2D(width, height, 1, 1, fmt, 1, 0, EAH_GPU_Read | EAH_GPU_Write);
		RenderViewPtr clr_view = rf.Make2DRenderView(*clr_tex, 0, 1, 0);
		screenBuffer->attach(FrameBuffer::ATT_Color0, clr_view);
		screenBuffer->attach(FrameBuffer::ATT_DepthStencil, dsView);


		engine.setDefaultFrameBuffer(mSceneFrameBuffer);

		for (size_t i = 0; i < mConfigs.size(); ++i)
		{
			PostProcessConfig const & config = mConfigs[i];
			PostProcesser* processer = createPostProcesser(config);
			Texture* colorTex = lastOutput->getAttached(FrameBuffer::ATT_Color0)->getSrcTexture();
			Texture* ds_tex = lastOutput->getAttached(FrameBuffer::ATT_DepthStencil)->getSrcTexture();

			FrameBufferPtr output;
			if (i == mConfigs.size() - 1)
			{
				output = screenBuffer;
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
					output->attach(FrameBuffer::ATT_Color0, rf.Make2DRenderView(*ct, 0, 1, 0));
				}
			}
			processer->setInputTexture(0, colorTex->shared_from_this());
			processer->setInputTexture(1, colorTex->shared_from_this());
			processer->setOutputFrameBuffer(output);
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
		std::function<void*(void)> m = PostProcessChain::PostProcessChain;

	}

	PostProcesser* PostProcessChain::createPostProcesser(PostProcessConfig config)
	{
		switch (config.mType)
		{
		case PPT_ToneMapping:
			return new ToneMapping();
		default:
			break;
		}
	}

	RenderablePtr const & PostProcessChain::getRenderable() const
	{
		return mRenderable;
	}
}