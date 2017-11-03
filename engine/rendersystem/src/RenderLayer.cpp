#include "Engine.h"
#include "rendersystem/include/RenderLayerPass.hpp"
#include "rendersystem/include/RenderLayer.hpp"

namespace Air
{
	void RenderLayer::update()
	{
		for (auto pass : mPasses)
		{
			pass->doPass();
		}
	}
}