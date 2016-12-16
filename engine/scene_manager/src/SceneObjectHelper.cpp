#include "Engine.h"
#include "rendersystem/include/Renderable.hpp"
#include "scene_manager/include/SceneObjectHelper.hpp"

namespace Air
{
	SceneObjectHelper::SceneObjectHelper(uint32_t attrib)
		:SceneObject(attrib)
	{

	}
	SceneObjectHelper::SceneObjectHelper(RenderablePtr const & renderable, uint32_t attrib)
		:SceneObject(attrib)
	{
		mRenderable = renderable;
		mRenderableHWResReady = mRenderable->getHWResourceReady();
		this->onAttachRenderable(false);
	}
	void SceneObjectHelper::onAttachRenderable(bool add_to_scene)
	{
	}


}