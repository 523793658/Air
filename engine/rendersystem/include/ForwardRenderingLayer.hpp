#ifndef _ForwardRenderingLayer_H_
#define _ForwardRenderingLayer_H_
#pragma once
#include "PreDeclare.h"
#include <functional>
#include "rendersystem/include/RenderLayer.hpp"

namespace Air
{

	class AIR_CORE_API ForwardRenderingLayer : public RenderLayer
	{
	public:
		ForwardRenderingLayer(RenderPipeline* pipeline);

		void loadFromXml(XMLNodePtr node);

		virtual void beforeUpdate() override;

		virtual void endUpdate() override;

	private:
		std::vector<SceneObject*> mLightObjects;
		std::vector<LightSourcePtr> mLights;
	};


}





#endif
