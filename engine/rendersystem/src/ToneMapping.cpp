
#include "Context.h"
#include "SingletonManager.hpp"
#include "basic/include/XMLDom.hpp"
#include "rendersystem/include/mesh.hpp"
#include "rendersystem/include/RenderEffect.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/ToneMapping.hpp"

namespace Air
{
	namespace
	{




		class ToneCreator : public PostProcesserCreator
		{
		public:
			std::shared_ptr<PostProcessConfig> loadCfg(XMLNodePtr const & node) override
			{
				std::shared_ptr<ToneMapping::ToneMappingConfig> config = MakeSharedPtr<ToneMapping::ToneMappingConfig>();
				config->mType = PPT_ToneMapping;
				config->mEffectPath = node->getAttribString("shader");
				return config;
			}

			PostProcesser* createInstance(std::shared_ptr<PostProcessConfig> const & config) override
			{
				ToneMapping* processer = new ToneMapping();
				processer->setConfig(config);
				return processer;
			}
		};
		RegisterProcesser("toneMapping", ToneCreator);
	}

	ToneMapping::ToneMapping()
	{

	}

	ToneMapping::~ToneMapping()
	{

	}

	void ToneMapping::update()
	{
		if (mDirty)
		{
			std::shared_ptr<ToneMappingConfig> cfg = checked_pointer_cast<ToneMappingConfig>(mConfig);
			mEffect = syncLoadRenderEffect(cfg->mEffectPath);
			mTechnique = mEffect->getTechniqueByName("toneMap");
			*mEffect->getParameterByName("u_SceneTex") = mSrcTextures[0];
			*mEffect->getParameterByName("u_LuminanceTex") = mSrcTextures[1];
			*mEffect->getParameterByName("u_Params") = float2(0, 1);
			*mEffect->getParameterByName("u_BlueShift") = 0;
			mDirty = false;
		}
	}

	void ToneMapping::render()
	{
		RenderEngine& re = SingletonManager::getRenderFactoryInstance().getRenderEngineInstance();
		re.bindFrameBuffer(mOutputFrameBuffer);
		mOutputFrameBuffer->discard(FrameBuffer::CBM_Color);

		StaticMesh & renderable = *checked_cast<StaticMesh*>(mConfig->mChain->getRenderable().get());
		renderable.setTechnique(mEffect, mTechnique);
		
		renderable.render();
	}
}