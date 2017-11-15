#include "Context.h"
#include "basic/include/Thread.h"
#include "rendersystem/include/TaskGraphInterfaces.hpp"
#include "rendersystem/include/SceneRenderTarget.hpp"
namespace Air
{
	static TGlobalResource<SceneRenderTargets> mSceneRenderTargetSingleton;

	SceneRenderTargets& SceneRenderTargets::get(CommandList& cmdList)
	{
		SceneRenderTargets* sceneContext = (SceneRenderTargets*)(cmdList.getRenderThreadContext(CommandListBase::ERenderThreadContext::SceneRenderTargets));

		if (!sceneContext)
		{
			return mSceneRenderTargetSingleton;
		}
		BOOST_ASSERT(!cmdList.isImmediate());
		return *sceneContext;
	}
	SceneRenderTargets& SceneRenderTargets::get(CommandListImmediate& cmdList)
	{
		BOOST_ASSERT((isInRenderingThread() && !cmdList.getRenderThreadContext(CommandListBase::ERenderThreadContext::SceneRenderTargets) && !TaskGraphInterface::get().isThreadProcessingTasks(ENamedThreads::RenderThread_Local)));
		return mSceneRenderTargetSingleton;
	}


}