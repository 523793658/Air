
#include "Context.h"
#include "SingletonManager.hpp"
#include "scene_manager/include/SceneManager.hpp"
#include "scene_manager/include/SceneObject.hpp"
#include "rendersystem/include/Renderable.hpp"
namespace Air
{
	SceneObject::SceneObject(uint32_t attrib)
		:mAttrib(attrib), mParent(nullptr), mRenderableHWResReady(false),
		mLocalMatrix(float4x4::identify()), mWorldMatrix(float4x4::identify()),
		mVisibleMark(BO_No)
	{
		if (!(attrib & SOA_Overlay) && (attrib & (SOA_Cullable | SOA_Moveable)))
		{
			mAABB = MakeSharedPtr<AABBox>();
		}
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

	void SceneObject::setCustomData(std::shared_ptr<void> data)
	{
		mCustomData = data;
	}

	std::shared_ptr<void> const & SceneObject::getCustomData() const
	{
		return mCustomData;
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
		return *mAABB;
	}

	bool SceneObject::mainThreadUpdate(float app_time, float elapsed_time)
	{
		bool refreshed = false;
		return refreshed;
	}

	void SceneObject::addToSceneManager()
	{
		SingletonManager::getSceneManagerInstance().addSceneObject(this->shared_from_this());
		for (auto const & child : mChildren)
		{
			child->addToSceneManager();
		}
	}

	void SceneObject::addToSceneManagerLocked()
	{
		SingletonManager::getSceneManagerInstance().addSceneObjectLocked(this->shared_from_this());
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

	AABBox const & SceneObject::getAABB() const
	{
		return *mAABB;
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
		else
		{
			mWorldMatrix = mLocalMatrix;
		}
		if (mRenderable)
		{
			if (mAABB)
			{
				*mAABB = MathLib::transform_aabb(mRenderable->getPosAABB(), mWorldMatrix);
			}
			mRenderable->setMatrix(mWorldMatrix);
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

	RenderablePtr const & SceneObject::getRenderable() const
	{
		return mRenderable;
	}
}