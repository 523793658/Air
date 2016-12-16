#ifndef _OCTREE_H_
#define _OCTREE_H_
#pragma once
#include "PreDeclare.h"
#include "scene_manager/include/SceneManager.hpp"
namespace Air
{
	class OCTree : public SceneManager
	{
	public:
		OCTree();

	private:
		virtual void onAddSceneObject(SceneObjectPtr const & obj);
		virtual void onDelSceneObject(std::vector<SceneObjectPtr>::iterator iter);

	private:
		bool mRebuildTree;

	};
}


#endif