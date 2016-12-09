#include "Engine.h"
#include "Context.h"
#include "scene_manager/include/SceneManager.hpp"
#include "scene_manager/include/SceneObject.hpp"
namespace Air
{
	SceneObject::SceneObject(uint32_t attrib)
	{

	}
	SceneObject::~SceneObject()
	{

	}

	SceneObject* SceneObject::getParent() const
	{
		return NULL;
	}
	void SceneObject::setParent(SceneObject* so)
	{

	}

	uint32_t SceneObject::getNumChildren() const
	{
		return 0;
	}

	const SceneObjectPtr& SceneObject::getChild(uint32_t index) const
	{
		return nullptr;
	}

	void SceneObject::setVisibleMark(BoundOverlap vm)
	{

	}

	bool SceneObject::mainThreadUpdate(float app_time, float elapsed_time)
	{
		bool refreshed = false;
		return refreshed;
	}

	void SceneObject::addToSceneManagerLocked()
	{
		Context::getInstance().getSceneManangerInstance().addSceneObjectLocked(this->shared_from_this());
		for (auto const & child : mChildren)
		{
			child->addToSceneManagerLocked();
		}
	}

	void SceneObject::onAttachRenderable(bool add_to_scene)
	{
		if (add_to_scene)
		{
			this->addToSceneManagerLocked();
		}
	}

	uint32_t SceneObject::getAttrib() const
	{
		return mAttrib;
	}

	void SceneObject::updateAbsModelMatrix()
	{
		if (mParent)
		{
			mWorldMatrix = mParent->getModelMatrix() * mLocalMatrix;
		}
	}

	float4x4 const & SceneObject::getModelMatrix() const
	{
		return mWorldMatrix;
	}
}