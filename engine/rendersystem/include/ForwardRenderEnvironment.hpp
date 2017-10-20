#ifndef _ForwardRenderEnvironment_H_
#define _ForwardRenderEnvironment_H_
#pragma once
#include "rendersystem/include/RenderEnvironment.hpp"
namespace Air
{
	class AIR_CORE_API ForwardRenderEnvironment : public RenderEnvironment
	{
	public:
		ForwardRenderEnvironment();
		virtual ~ForwardRenderEnvironment();
		void update();
	};
}




#endif
