#include "Engine.h"
#include "rendersystem/include/mesh.hpp"
#include "rendersystem/include/RenderEffect.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/ToneMapping.hpp"

namespace Air
{
	namespace
	{

		struct ToneMappingConfig : public PostProcessConfig
		{

		};


		class ToneCreator : public PostProcesserCreator
		{
		public:
			std::shared_ptr<PostProcessConfig> loadCfg(XMLNodePtr const & node) override
			{
				std::shared_ptr<PostProcessConfig> config = MakeSharedPtr<ToneMappingConfig>();
				config->mType = PPT_ToneMapping;




				return config;
			}

			PostProcesser* createInstance(std::shared_ptr<PostProcessConfig> const &, PostProcessChain* chain) override
			{
				ToneMapping* processer = new ToneMapping(chain);
				
				return processer;
			}
		};
	}

	RegisterProcesser("toneMap", ToneCreator);


	ToneMapping::ToneMapping(PostProcessChain* chain)
	{
		mChain = chain;
		
		mEffect = syncLoadRenderEffect("assets/shader/toneMap.asd");
		mTechnique = mEffect->getTechniqueByName("toneMap");
		mSrcColorTextureParam = mEffect->getParameterByName("u_SceneTex");
	}

	ToneMapping::~ToneMapping()
	{

	}

	void ToneMapping::update()
	{
		*mSrcColorTextureParam = mSrcTextures[0];
	}

	void ToneMapping::render()
	{
		RenderEngine& re = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		re.bindFrameBuffer(mOutputFrameBuffer);
		mOutputFrameBuffer->discard(FrameBuffer::CBM_Color);

		StaticMesh & renderable = *checked_cast<StaticMesh*>(mChain->getRenderable().get());
		renderable.setTechnique(mEffect, mTechnique);
		
		renderable.render();
	}
}