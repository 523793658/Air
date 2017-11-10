#ifndef _RenderLayerPass_H_
#define _RenderLayerPass_H_
#pragma once
#include "PreDeclare.h"

namespace Air
{
	enum RenderLayerPassType
	{
		RLPT_ShadowMap,
		RLPT_ForwardRendering,
		RLPT_SkyBox,
		RLPT_Helper,
	};


	class AIR_CORE_API RenderLayerPass
	{
	public:

		void setRenderTarget(FrameBufferPtr const & target);


		virtual void doPass();

		virtual void initialize();

	protected:
		virtual void addToRenderQueue(Renderable* obj);

		virtual void prepareObject() = 0;

		virtual void prepareRenderQueue();

		virtual void flushQueue();

	protected:
		std::vector<SceneObject*> mSceneObjects;
		std::vector<Renderable*> mRenderQueue;
		FrameBufferPtr mRenderTarget;
	};
}







#endif
