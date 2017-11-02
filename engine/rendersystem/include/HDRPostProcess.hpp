#ifndef _HDRPostProcess_H_
#define _HDRPostProcess_H_
#pragma once

#include "PreDeclare.h"
#include "rendersystem/include/PostProcess.hpp"

namespace Air
{
	class AIR_CORE_API Luminance : public PostProcesser
	{
	public:
		Luminance();
		~Luminance();

		virtual void update();
		virtual void render();
	protected:
		RenderEffectPtr mEffect;
		RenderTechnique* mTechnique;
	};
	typedef std::shared_ptr<Luminance> LuminancePtr;

	class AIR_CORE_API LuminanceCS : public Luminance
	{
	public:
		LuminanceCS();
		~LuminanceCS();
		void update();
		void render();
	private:
		TexturePtr m2DOuputTexture;
	};

	class AIR_CORE_API BloomProcesser : public PostProcesser
	{
	public:

		virtual void update() override;
		virtual void render() override;

	};

	class AIR_CORE_API HDRPostProcesser : public PostProcesser
	{
	public:
		HDRPostProcesser();
		virtual ~HDRPostProcesser();

		virtual void update();
		virtual void render();

		void HDRPostProcesser::setConfig(PostProcessConfigPtr config) override;

	private:
		PostProcesserPtr mLuminance;
		PostProcesserPtr mBloom;
		PostProcesserPtr mTonemap;

		TexturePtr mLuminanceTex;
		TexturePtr mBloomTex;
	};
}



#endif
