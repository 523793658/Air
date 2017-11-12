#ifndef _DeferredShadingRenderer_H_
#define _DeferredShadingRenderer_H_
#pragma once
#include "rendersystem/include/SceneRenderer.hpp"
namespace Air
{
	class DeferredShadingSceneRenderer : public SceneRenderer
	{
	public:
		virtual void render(CommandListImmediate& cmdList) override;

	protected:
		bool initViews(CommandListImmediate& cmdList, struct FILCUpdatePrimTaskData& ILCTaskData);
	};













}



#endif
