#ifndef _ToneMapping_H_
#define _ToneMapping_H_
#pragma once
#include "rendersystem/include/PostProcess.hpp"
namespace Air
{
	class AIR_CORE_API ToneMapping : public PostProcesser
	{
	public:
		struct ToneMappingConfig : public PostProcessConfig
		{
			std::string mEffectPath;
		};
	public:
		ToneMapping();
		~ToneMapping();


		void update() override;
		void render() override;
	private:
		RenderEffectPtr mEffect;
		RenderTechnique* mTechnique;
		RenderEffectParameter* mSrcColorTextureParam;
	};
}



#endif
