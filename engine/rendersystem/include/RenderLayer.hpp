#ifndef _RenderLayer_H_
#define _RenderLayer_H_
#include "PreDeclare.h"
namespace Air
{
	class AIR_CORE_API RenderLayer : boost::noncopyable
	{
	public:
		RenderLayer(RenderPipeline* pipeline);
		virtual ~RenderLayer() = 0;
		
		virtual void update();

		virtual void loadFromXml(XMLNodePtr node) = 0;

		virtual void beforeUpdate();

		virtual void endUpdate();

	protected:
		std::vector<RenderLayerPassPtr> mPasses;
		RenderPipeline* mPipeline;
	};
}



#endif
