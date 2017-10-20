#include "Engine.h"
#include "Camera.hpp"
#include "app/include/App3D.hpp"
#include "scene_manager/include/SceneManager.hpp"
#include "scene_manager/include/SceneObject.hpp"
#include "OCTree.hpp"
#define MAX_NODE_SCENE_OBJECT 10


namespace Air
{

	OCTree::OCTree(AABBox&& size)
	{
		mNodes.resize(1);
		mNodes[0].mAABB = size;
	} 

	OCTree::OCTree(AABBox const & size)
	{
		mNodes.resize(1);
		mNodes[0].mAABB = size;
	}

	void OCTree::markNodeVisible(size_t index)
	{
		App3DFramework& app = Engine::getInstance().getAppInstance();
		Camera& camera = app.getActiveCamera();
		float4x4 view_proj = camera.getViewProjMatrix();
		OCTreeNode& node = mNodes[index];
		if ((mSmallObjThreshold <= 0) ||
			((MathLib::ortho_area(camera.getForwardVec(), node.mAABB)) > mSmallObjThreshold) && (MathLib::perspective_area(camera.getEyePos(), view_proj, node.mAABB) > mSmallObjThreshold))
		{
			BoundOverlap const vis = mFrustum->intersect(node.mAABB);
			node.mVisible = vis;
			if (BO_Partial == vis)
			{
				if (node.mFirstChildIndex >= 0)
				{
					for (int i = 0; i < 8; i++)
					{
						this->markNodeVisible(node.mFirstChildIndex + i);
					}
				}
			}
		}
		else
		{
			node.mVisible = BO_No;
		}
	}

	void OCTree::markNodeObjs(size_t index, bool force)
	{
		BOOST_ASSERT(index < mNodes.size());
		App3DFramework& app = Engine::getInstance().getAppInstance();
		Camera& camera = app.getActiveCamera();
		float4x4 view_proj = camera.getViewProjMatrix();
		OCTreeNode const & node = mNodes[index];
		if ((node.mVisible == BO_Partial) || force)
		{
			for (auto const & so : node.mSceneObjects)
			{
				if ((BO_No == so->getVisibleMark()) && so->isVisible())
				{
					BoundOverlap visible = this->visibleTestFromParent(so, camera.getForwardVec(), camera.getEyePos(), view_proj);
					if (BO_Partial == visible)
					{
						AABBox const & aabb = so->getAABB();
						if (so->getParent() || (mSmallObjThreshold <= 0) || ((MathLib::ortho_area(camera.getForwardVec(), node.mAABB) > mSmallObjThreshold) && (MathLib::perspective_area(camera.getEyePos(), view_proj, aabb) > mSmallObjThreshold)))
						{
							visible = mFrustum->intersect(aabb);
						}
						else
						{
							visible = BO_No;
						}
					}
					so->setVisibleMark(visible);
				}
			}
			if (node.mFirstChildIndex >= 0)
			{
				for (int i = 0; i < 8; ++i)
				{
					this->markNodeObjs(node.mFirstChildIndex + i, force);
				}
			}
		}
		else if (node.mVisible == BO_Yes)
		{
			for (auto const & so : node.mSceneObjects)
			{
				so->setVisibleMark(BO_Yes);
			}
			if (node.mFirstChildIndex >= 0)
			{
				for (int i = 0; i < 8; ++i)
				{
					this->markNodeObjs(node.mFirstChildIndex + i, true);
				}
			}
		}
	}

	void OCTree::clipScene()
	{
		for (auto it : mNodes)
		{
			it.mVisible = BO_No;
		}

		if (!mNodes.empty())
		{
			this->markNodeVisible(0);
		}
		App3DFramework& app = Engine::getInstance().getAppInstance();
		Camera & camera = app.getActiveCamera();
		float4x4 view_proj = camera.getViewProjMatrix();
		if (camera.getOmniDirectionalMode())
		{

		}
		else
		{
			if (!mNodes.empty())
			{
				this->markNodeObjs(0, false);
			}
		}
		
	}

	bool OCTree::addObjToNode(int index, SceneObject * obj)
	{
		//能否包含
		if (mNodes[index].mAABB.contains(obj->getAABB()))
		{
			//是叶子节点
			if (mNodes[index].mFirstChildIndex < 0)
			{
				//是否需要分裂
				if (mNodes[index].mSceneObjects.size() < 10)
				{
					mNodes[index].mSceneObjects.push_back(obj);
					obj->setSpaceNodeHandle(index);
				}
				else
				{
					mNodes[index].mFirstChildIndex = mNodes.size();
					mNodes.resize(mNodes.size() + 8);
					OCTreeNode& parent_node = mNodes[index];
					float3 parentCenter = parent_node.mAABB.getCenter();
					for (int i = 0; i < 8; ++i)
					{
						OCTreeNode& new_node = mNodes[mNodes[index].mFirstChildIndex + i];
						new_node.mFirstChildIndex = -1;
						new_node.mAABB = AABBox(float3((i & 1) ? parentCenter.x() : parent_node.mAABB.getMin().x(),
							(i & 2) ? parentCenter.y
							() : parent_node.mAABB.getMin().y(),
							(i & 4) ? parentCenter.z() : parent_node.mAABB.getMin().z()), float3((i & 1) ? parent_node.mAABB.getMax().x() : parentCenter.x(),
							(i & 2) ? parent_node.mAABB.getMax().y() : parentCenter.y(), (i & 4) ? parent_node.mAABB.getMax().z() : parentCenter.z()));
					}
					std::vector<SceneObject*> nodes = mNodes[index].mSceneObjects;
					nodes.push_back(obj);
					mNodes[index].mSceneObjects.clear();
					for (auto const & o : nodes)
					{
						addObjToNode(index, o);
					}
				}
			}
			else
			{
				float3 p = obj->getAABB().getCenter();
				float3 p2 = mNodes[index].mAABB.getCenter();
				int childIndex = (p.x() > p2.x()) | ((p.y() > p2.y()) * 2) | ((p.z() > p2.z()) * 4);
				if (!addObjToNode(childIndex + mNodes[index].mFirstChildIndex, obj))
				{
					mNodes[index].mSceneObjects.push_back(obj);
					obj->setSpaceNodeHandle(index);
				}
			}
			return true;
		}
		else
		{
			return false;
		}
		
	}

	void OCTree::onAddSceneObject(SceneObjectPtr const & obj)
	{
		addObjToNode(0, obj.get());
	}
	void OCTree::onDelSceneObject(std::vector<SceneObjectPtr>::iterator iter)
	{
		OCTreeNode & node = mNodes[(*iter)->getSpaceNodeHandle()];
		auto it = std::find(node.mSceneObjects.begin(), node.mSceneObjects.end(), iter->get());
		if (it != node.mSceneObjects.end())
		{
			node.mSceneObjects.erase(it);
		}
	}
}
