#include "Engine.h"
#include "scene_manager/include/SceneManager.hpp"
#include "scene_manager/include/SceneObject.hpp"
#include "OCTree.hpp"
namespace Air
{
	OCTree::OCTree()
	{
		
	}

	void OCTree::onAddSceneObject(SceneObjectPtr const & obj)
	{
		uint32_t const attr = obj->getAttrib();
		if ((attr & SceneObject::SOA_Cullable) && !(attr & SceneObject::SOA_Moveable))
		{
			mRebuildTree = true;
		}
	}
	void OCTree::onDelSceneObject(std::vector<SceneObjectPtr>::iterator iter)
	{
		BOOST_ASSERT(iter != mSceneObjs.end());
		uint32_t const attr = (*iter)->getAttrib();
		if ((attr & SceneObject::SOA_Cullable) && !(attr & SceneObject::SOA_Moveable))
		{
			mRebuildTree = true;
		}

	}
}
