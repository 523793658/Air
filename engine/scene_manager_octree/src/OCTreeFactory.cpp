#include "Engine.h"
#include "basic/include/Util.h"
#include "OCTree.hpp"
#include "OCTreeFactory.hpp"

AIR_OCTREE_SM_API void makeSceneManager(std::unique_ptr<Air::SceneManager>& ptr)
{
	ptr = Air::MakeUniquePtr<Air::OCTree>();
}