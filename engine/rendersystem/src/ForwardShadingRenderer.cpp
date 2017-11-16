#include "Context.h"
#include "rendersystem/include/SceneView.hpp"
#include "rendersystem/include/SceneViewExtension.hpp"
#include "core/include/CoreGlobals.hpp"
#include "rendersystem/include/CommandList.hpp"
#include "rendersystem/include/ScenePrivate.hpp"
#include "rendersystem/include/SceneUtil.hpp"
#include "rendersystem/include/SceneRenderTarget.hpp"
#include "rendersystem/include/ForwardShadingRenderer.hpp"
namespace Air
{
	void ForwardSceneRenderer::initViews(CommandListImmediate& cmdList)
	{
		for (int32_t viewIndex = 0; viewIndex < mViews.size(); ++viewIndex)
		{
			mViews[viewIndex].initHardwareResource();
			createDirectionalLightUniformBuffers(mViews[viewIndex]);
		}
		onStartFrame(cmdList);
	}

	void ForwardSceneRenderer::createDirectionalLightUniformBuffers(SceneView& sceneView)
	{
		bool bDynamicShadows;
		//sceneView.mForwardDirectionalLightUniformBuffers[0] = 
	}

	void ForwardSceneRenderer::render(CommandListImmediate& cmdList)
	{

		SceneRenderTargets& sceneContext = SceneRenderTargets::get(cmdList);


		initViews(cmdList);
		for (auto viewExt : mViewFamily.mViewExtensions)
		{
			viewExt->postInitViewFamily_RenderThread(cmdList, mViewFamily);
			for (int32_t viewIndex = 0; viewIndex < mViewFamily.mViews.size(); ++viewIndex)
			{
				viewExt->postInitView_RenderThread(cmdList, mViews[viewIndex]);
			}
		}
		if (GRenderingThread)
		{
			CommandListExecutor::getImmediateCommandList().immediateFlush(EImmediateFlushType::FlushRHIThreadFlushResources);
		}

		//if(mScene->mFXSystem && !mViews[0].bIsPlanarReflection && mViewFamily.)

		ViewInfo& view = mViews[0];
		const bool bGammaSpace = !isForwardHDR();

		const bool bRequiresUpscale = false;

		const bool bStereoRenderingAndHMD = false;

		const bool bRenderToSceneColor = bStereoRenderingAndHMD || bRequiresUpscale || view.bIsSceneCapture || view.bIsReflectionCapture;

		RHITexture* sceneColor = nullptr;
		if (bGammaSpace && !bRenderToSceneColor)
		{
			//sceneColor = (view.bIsMobileMultiViewEnabled) ? sceneContext.
		}

	}
}