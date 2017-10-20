#ifndef _SkyBox_H_
#define _SkyBox_H_
#pragma once
#include "PreDeclare.h"
#include "rendersystem/include/RenderableHelper.hpp"
namespace Air

{
	class AIR_CORE_API RenderableSkyBox : public RenderableHelper
	{
	public:
		RenderableSkyBox();
		virtual ~RenderableSkyBox()
		{

		}

		virtual void setTechnique(RenderEffectPtr const & effect, RenderTechnique* tech);
		void setCubeMap(TexturePtr const & cube);

		void setCompressedCubeMap(TexturePtr const & y_cube, TexturePtr const & c_cube);

		void onRenderBegin();

		virtual void setPass(PassType type);

	protected:
		RenderEffectParameter* mDepthFar;
		RenderEffectParameter* mInvMVP;
		RenderEffectParameter* mSkyCubeTex;
		RenderEffectParameter* mSkyCcubeTex;
		RenderEffectParameter* mSkyCompressed;
	};


}



#endif
