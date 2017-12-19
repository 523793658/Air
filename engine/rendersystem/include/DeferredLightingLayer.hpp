#ifndef _DefferredLightingLayer_H_
#define _DefferredLightingLayer_H_
#pragma once
#include "rendersystem/include/RenderLayer.hpp"
namespace Air
{
	class DeferredLightingLayer : public RenderLayer
	{
		DeferredLightingLayer(RenderPipeline* pipeline);

		void loadFromXml(XMLNodePtr node);

		virtual void beforeUpdate() override;

		virtual void endUpdate() override;

	private:
	};
}
#endif
