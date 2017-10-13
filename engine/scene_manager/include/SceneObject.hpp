#ifndef _Scene_Manager_H_
#define _Scene_Manager_H_
#pragma  once

#include "PreDeclare.h"
#include <memory>
namespace Air
{
	class AIR_CORE_API SceneObject : public std::enable_shared_from_this<SceneObject>
	{
	public:
		enum SOAttrib
		{
			SOA_Cullable = 1UL << 0,
			SOA_Overlay = 1UL << 1,
			SOA_Moveable = 1UL << 2,
			SOA_Invisible = 1UL << 3,
			SOA_NotCastShadow = 1UL << 4,
			SOA_SSS = 1UL << 5
		};
	public:
		explicit SceneObject(uint32_t attrib);
		virtual ~SceneObject();

		SceneObject* getParent() const;
		void setParent(SceneObject* so);

		uint32_t getNumChildren() const;

		uint32_t getAttrib() const;

		const SceneObjectPtr& getChild(uint32_t index) const;

		void setVisibleMark(BoundOverlap vm);
		BoundOverlap getVisibleMark() const;

		AABBox& getAABB();

		bool isVisible() const;

		virtual float4x4 const & getLocalMatrix() const;
		virtual void setLocalMatrix(float4x4 const &mat);

		virtual float4x4 const & getWorldMatrix() const;

		bool mainThreadUpdate(float app_time, float elapsed_time);

		virtual void onAttachRenderable(bool add_to_scene);

		virtual void addToSceneManager();

		virtual void addToSceneManagerLocked();

		void updateWorldMatrix();

		AABBox const & getAABB() const;

		RenderablePtr const & getRenderable() const;

		void setSpaceNodeHandle(int handle)
		{
			mSpaceNodeHandle = handle;
		}

		int const getSpaceNodeHandle() const
		{
			return mSpaceNodeHandle;
		}

	protected:
		uint32_t mAttrib;

		SceneObject* mParent;
		std::vector<SceneObjectPtr> mChildren;

		RenderablePtr mRenderable;
		bool mRenderableHWResReady;

		float4x4 mWorldMatrix;
		float4x4 mLocalMatrix;

		BoundOverlap mVisibleMark;

		AABBoxPtr mAABB;

		int mSpaceNodeHandle;
	};
}



#endif // !_Scene_Manager_H_
