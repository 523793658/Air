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

		virtual float4x4 const & getModelMatrix() const;

		bool mainThreadUpdate(float app_time, float elapsed_time);

		virtual void onAttachRenderable(bool add_to_scene);

		virtual void addToSceneManagerLocked();

		void updateAbsModelMatrix();
	protected:
		uint32_t mAttrib;

		SceneObject* mParent;
		std::vector<SceneObjectPtr> mChildren;

		float4x4 mWorldMatrix;
		float4x4 mLocalMatrix;
	};
}



#endif // !_Scene_Manager_H_
