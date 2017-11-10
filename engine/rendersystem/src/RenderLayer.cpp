#include "Context.h"
#include "rendersystem/include/RenderLayerPass.hpp"
#include "rendersystem/include/RenderLayer.hpp"

namespace Air
{
	RenderLayer::RenderLayer(RenderPipeline* pipeline)
	{
		mPipeline = pipeline;
	}

	void RenderLayer::beforeUpdate()
	{

	}

	void RenderLayer::endUpdate()
	{

	}

	void RenderLayer::update()
	{
		this->beforeUpdate();
		for (auto pass : mPasses)
		{
			pass->doPass();
		}
		this->endUpdate();
	}

	RenderLayer::~RenderLayer()
	{

	}
}