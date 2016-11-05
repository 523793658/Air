#include "Engine.h"
#include "rendersystem/include/RenderFactory.h"


namespace Air
{
	RenderFactory::~RenderFactory()
	{
	}

	RenderEngine& RenderFactory::getRenderEngineInstance()
	{
		if (!mRenderEnginePtr)
		{
			mRenderEnginePtr = this->doMakeRenderEngine();
		}
		return *mRenderEnginePtr;
	}
}