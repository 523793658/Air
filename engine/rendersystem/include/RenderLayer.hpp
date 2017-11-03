#ifndef _RenderLayer_H_
#define _RenderLayer_H_
#include "PreDeclare.h"
namespace Air
{
	class AIR_CORE_API RenderLayer : boost::noncopyable
	{
	public:
		RenderLayer();
		virtual ~RenderLayer() = 0;
		
		virtual void update();

	protected:
		std::vector<RenderLayerPassPtr> mPasses;
	};
}



#endif
