#ifndef _ForwardRenderingLayer_H_
#define _ForwardRenderingLayer_H_
#pragma once
#include "PreDeclare.h"
#include <functional>


namespace Air
{
	class AIR_CORE_API RenderingLayer : boost::noncopyable
	{
	public:
		virtual void update(uint32_t pass) = 0;
	};

	class AIR_CORE_API ForwardRenderingLayer : public RenderingLayer
	{
	public:
		ForwardRenderingLayer();

		virtual void update(uint32_t pass) override;

	private:
		std::vector<LightSourcePtr> mLights;
	};


}





#endif
