#include "Context.h"
#include "rendersystem/include/SceneRenderTarget.hpp"
#include "rendersystem/include/SceneManagerment.hpp"
#include "rendersystem/include/SceneRenderer.hpp"
namespace Air
{
	void ViewInfo::initHardwareResource()
	{

	}
	void SceneRenderer::onStartFrame(CommandListImmediate& cmdList)
	{
		SceneRenderTargets& SceneContext = SceneRenderTargets::get(cmdList);
		
		for (auto view : mViews)
		{
			SceneViewStateInterface* state = view.mState;
			if (state)
			{
				state->onStartFrame(view, mViewFamily);
			}
		}
	}


}