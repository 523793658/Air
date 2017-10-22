#include "Engine.h"
#include "rendersystem/include/mesh.hpp"
#include "rendersystem/include/RenderEffect.hpp"
#include "rendersystem/include/ToneMapping.hpp"

namespace Air
{
	ToneMapping::ToneMapping()
	{

	}

	ToneMapping::~ToneMapping()
	{

	}

	void ToneMapping::update()
	{
		*mSrcColorTextureParam = mSrcTextures[0];
	}

	void ToneMapping::render()
	{
		StaticMesh & renderable = *checked_cast<StaticMesh*>(mChain->getRenderable().get());
		renderable.setTechnique(mEffect, mTechnique);
		renderable.render();
	}
}