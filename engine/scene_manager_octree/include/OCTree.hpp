#ifndef _OCTREE_H_
#define _OCTREE_H_
#pragma once
#include "PreDeclare.h"
#include "scene_manager/include/SceneManager.hpp"
namespace Air
{

	class OCTree : public SceneManager
	{
		class OCTreeNode
		{
		public:
			AABBox mAABB;
			int mFirstChildIndex{ -1 };
			BoundOverlap mVisible;
			std::vector<SceneObject*> mSceneObjects;
			OCTreeNode* mParent;
		public:
			void addSceneObject(SceneObject* sceneObj);
		};

	public:
		OCTree(AABBox &&size);
		OCTree(AABBox const & size);
		virtual void clipScene() override;

	protected:
		virtual void onAddSceneObject(SceneObjectPtr const & obj);

		virtual void onDelSceneObject(std::vector<SceneObjectPtr>::iterator iter);

		bool addObjToNode(int index, SceneObject * obj);


	private:
		void markNodeVisible(size_t index);
		void markNodeObjs(size_t index, bool force);
	private:
		std::vector<OCTreeNode> mNodes;
	};
}


#endif