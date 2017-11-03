#ifndef _RenderLayerPass_H_
#define _RenderLayerPass_H_
#pragma once
#include "PreDeclare.h"

namespace Air
{
	class AIR_CORE_API RenderLayerPass
	{
	public:
		virtual void doPass() = 0;
	};
}







#endif
