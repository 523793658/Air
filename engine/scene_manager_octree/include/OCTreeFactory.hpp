#ifndef _OCTREE_FACTORY_H_
#define _OCTREE_FACTORY_H_
#pragma once

#include "PreDeclare.h"
#ifdef AIR_SCENE_MANAGER_OC_TREE_SOURCE
#define AIR_OCTREE_SM_API AIR_SYMBOL_EXPORT
#else
#define AIR_OCTREE_SM_API AIR_SYMBOL_IMPORT
#endif

extern "C"
{
	AIR_OCTREE_SM_API void makeSceneManager(std::unique_ptr<Air::SceneManager>& ptr);
}


#endif