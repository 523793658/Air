#ifndef _SceneObjectHelper_H_
#define _SceneObjectHelper_H_
#pragma once

#include "scene_manager/include/SceneObject.hpp"

namespace Air
{
	class AIR_CORE_API SceneObjectHelper : public SceneObject
	{
	public:
		explicit SceneObjectHelper(uint32_t attrib);
		SceneObjectHelper(RenderablePtr const & renderable, uint32_t attrib);
		virtual ~SceneObjectHelper()
		{
		}
		virtual void onAttachRenderable(bool add_to_scene) override;

	};

	class AIR_CORE_API SceneObjectSkyBox : public SceneObjectHelper
	{
	public:
		explicit SceneObjectSkyBox(uint32_t attrib = 0);
		virtual ~SceneObjectSkyBox()
		{

		}
		void setTechnique(RenderEffectPtr const & effect, RenderTechnique *);

		void setCubeMap(TexturePtr const & cube);

		void setCompressedCubeMap(TexturePtr const & y_cube, TexturePtr const & c_cube);
	};
}


#endif