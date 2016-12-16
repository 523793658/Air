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
		BOOST_ASSERT(index < mChildren.size());
		return mChildren[index];
	}


	bool SceneObject::isVisible() const
	{
		return (0 == (mAttrib & SOA_Invisible));
	}
	void SceneObject::setVisibleMark(BoundOverlap vm)
	{
		mVisibleMark = vm;
	}

	BoundOverlap SceneObject::getVisibleMark() const
	{
		return mVisibleMark;
	}

	AABBox& SceneObject::getAABB()
	{
		AABBox& a = *mAABB;
	}

	bool SceneObject::mainThreadUpdate(float app_time, float elapsed_time)
	{
		bool refreshed = false;
		return refreshed;
	}

	void SceneObject::addToSceneManager()
	{
		Context::getInstance().getSceneManangerInstance().addSceneObject(this->shared_from_this());
		for (auto const & child : mChildren)
		{
			child->addToSceneManager();
		}
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

	void SceneObject::updateWorldMatrix()
	{
		if (mParent)
		{
			mWorldMatrix = mParent->getWorldMatrix() * mLocalMatrix;
		}
	}

	float4x4 const & SceneObject::getLocalMatrix() const
	{
		return mWorldMatrix;
	}

	void SceneObject::setLocalMatrix(float4x4 const & mat)
	{
		mLocalMatrix = mat;
	}

	float4x4 const & SceneObject::getWorldMatrix() const
	{
		return mWorldMatrix;
	}

	Renderable const & SceneObject::getRenderable() const
	{
		return *mRenderable;
	}
}