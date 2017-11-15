#ifndef _ForwardShadingRenderer_H_
#define _ForwardShadingRenderer_H_
#pragma once
#include "basic/include/Color.hpp"
#include "rendersystem/include/SceneRenderer.hpp"
#include "rendersystem/include/DynamicRHI.hpp"
namespace Air
{
	



	class ForwardSceneRenderer : public SceneRenderer
	{
	public:
		virtual void render(CommandListImmediate& cmdList) override;

	protected:
		void initViews(CommandListImmediate& cmdList);

		void createDirectionalLightUniformBuffers(SceneView& sceneView);
	};
}
#endif
