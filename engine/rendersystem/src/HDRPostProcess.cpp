#include "Engine.h"
#include "basic/include/Math.hpp"

#include "basic/include/XMLDom.hpp"

#include "rendersystem/include/ToneMapping.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/PostProcess.hpp"

#include "rendersystem/include/HDRPostProcess.hpp"

#define ThreadGroupSize 8
#define GroupCount 16


namespace Air
{
	Luminance::Luminance()
	{

	}
	Luminance::~Luminance()
	{

	}

	void Luminance::update()
	{
	
	}
	void Luminance::render()
	{

	}


	namespace
	{
		struct LuminanceCSConfig : public PostProcessConfig
		{
			std::string mEffectPath;
		};

		class LuminanceCSCreator : public PostProcesserCreator
		{
			std::shared_ptr<PostProcessConfig> loadCfg(XMLNodePtr const & node) override
			{
				std::shared_ptr<LuminanceCSConfig> config = MakeSharedPtr<LuminanceCSConfig>();
				config->mEffectPath = node->getAttribString("shader");
				return config;
			}

			PostProcesser* createInstance(std::shared_ptr<PostProcessConfig> const & cfg) override
			{
				LuminanceCS* p = new LuminanceCS();
				p->setConfig(cfg);
				return p;
			}
		};
		RegisterProcesser("luminanceCS", LuminanceCSCreator);
	}


	LuminanceCS::LuminanceCS()
	{
		RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();

		m2DOuputTexture = rf.MakeTexture2D(GroupCount, GroupCount, 1, 1, EF_R32F, 1, 0, EAH_GPU_Read | EAH_GPU_Write | EAH_GPU_Unordered);
	}
	LuminanceCS::~LuminanceCS()
	{

	}



	void LuminanceCS::update()
	{
		if (mDirty)
		{
			uint32_t width, height;
			width = mSrcTextures[0]->getWidth();
			height = mSrcTextures[0]->getHeight();
			std::shared_ptr<LuminanceCSConfig> cfg = checked_pointer_cast<LuminanceCSConfig>(mConfig);
			mEffect = syncLoadRenderEffect(cfg->mEffectPath);
			mTechnique = mEffect->getTechniqueByName("luminanceCS");
			*mEffect->getParameterByName("u_SceneColorTex") = mSrcTextures[0];
			*mEffect->getParameterByName("u_Lum2DTexOutput") = m2DOuputTexture;
			*mEffect->getParameterByName("u_LuminanceParams") = uint4(width, height, GroupCount, 0);
			*mEffect->getParameterByName("u_LuminanceTex") = mOutputTexture[0];
			mDirty = false;
		}
	}
	void LuminanceCS::render()
	{
		RenderEngine& engine = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		//Éú³É1DÎÆÀí
		this->onRenderBegin();
		engine.dispatch(*mEffect, *mTechnique, GroupCount, GroupCount, 1);
		this->onRenderEnd();
	}

	void BloomProcesser::update()
	{

	}
	void BloomProcesser::render()
	{

	}

	namespace
	{
		struct HDRConfig : public PostProcessConfig
		{
			std::shared_ptr<PostProcessConfig> mToneMappingConfig;
			std::shared_ptr<PostProcessConfig> mLuminanceConfig;
			std::shared_ptr<PostProcessConfig> mBloomConfig;
		};

		class HDRCreator : public PostProcesserCreator
		{
		public:
			std::shared_ptr<PostProcessConfig> loadCfg(XMLNodePtr const & node) override
			{
				std::shared_ptr<HDRConfig> config = MakeSharedPtr<HDRConfig>();

				XMLNodePtr luminanceNode = node->getFirstNode("luminanceConfig");
				std::string luminanceName = luminanceNode->getAttribString("name");
				PostProcesserCreator* creator = PostProcessChain::getProcesserCreator(luminanceName);
				config->mLuminanceConfig = creator->loadCfg(luminanceNode);


				XMLNodePtr toneMappingNode = node->getFirstNode("toneMapping");
				std::string toneMappingName = toneMappingNode->getAttribString("name");
				creator = PostProcessChain::getProcesserCreator(toneMappingName);
				config->mToneMappingConfig = creator->loadCfg(toneMappingNode);


				return config;
			}

			PostProcesser* createInstance(std::shared_ptr<PostProcessConfig> const & cfg) override
			{
				HDRPostProcesser* processer = new HDRPostProcesser();
				processer->setConfig(cfg);
				return processer;
			}
		};

		RegisterProcesser("hdr", HDRCreator);

	}

	void HDRPostProcesser::setConfig(PostProcessConfigPtr config)
	{
		PostProcesser::setConfig(config);
		std::shared_ptr<HDRConfig> cfg = checked_pointer_cast<HDRConfig>(config);
		cfg->mToneMappingConfig->mChain = config->mChain;
		cfg->mLuminanceConfig->mChain = config->mChain;
		mLuminance->setConfig(cfg->mLuminanceConfig);
		mTonemap->setConfig(cfg->mToneMappingConfig);
	}

	HDRPostProcesser::HDRPostProcesser()
	{
		RenderFactory & factory = Engine::getInstance().getRenderFactoryInstance();
		mLuminance = MakeSharedPtr<LuminanceCS>();
		mBloom = MakeSharedPtr<BloomProcesser>();
		mTonemap = MakeSharedPtr<ToneMapping>();

		mLuminanceTex = factory.MakeTexture1D(1, 1, 1, EF_R32F, 1, 0, EAH_GPU_Read | EAH_GPU_Unordered | EAH_GPU_Write);
	}
	HDRPostProcesser::~HDRPostProcesser()
	{

	}

	void HDRPostProcesser::update()
	{
		if (mDirty)
		{
			mLuminance->setOuputTexture(0, mLuminanceTex);
			mLuminance->setInputTexture(0, mSrcTextures[0]);

			mTonemap->setInputTexture(0, mSrcTextures[0]);
			mTonemap->setInputTexture(1, mLuminanceTex);
			mTonemap->setOutputFrameBuffer(mOutputFrameBuffer);
			mDirty = false;
		}

		mLuminance->update();
		mTonemap->update();

	}
	void HDRPostProcesser::render()
	{
		mLuminance->render();
		mTonemap->render();
	}

}