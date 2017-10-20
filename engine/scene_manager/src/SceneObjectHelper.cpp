#include "Engine.h"
#include "rendersystem/include/Renderable.hpp"
#include "rendersystem/include/SkyBox.hpp"
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


	SceneObjectSkyBox::SceneObjectSkyBox(uint32_t attrib /* = 0 */)
		:SceneObjectHelper(MakeSharedPtr<RenderableSkyBox>(), attrib | SOA_NotCastShadow)
	{
		
	}

	void SceneObjectSkyBox::setTechnique(RenderEffectPtr const & effect, RenderTechnique * tech)
	{
		checked_pointer_cast<RenderableSkyBox>(mRenderable)->setTechnique(effect, tech);
	}
	void SceneObjectSkyBox::setCubeMap(TexturePtr const & cube)
	{
		checked_pointer_cast<RenderableSkyBox>(mRenderable)->setCubeMap(cube);
	}

	void SceneObjectSkyBox::setCompressedCubeMap(TexturePtr const & y_cube, TexturePtr const & c_cube)
	{
		checked_pointer_cast<RenderableSkyBox>(mRenderable)->setCompressedCubeMap(y_cube, c_cube);
	}
}