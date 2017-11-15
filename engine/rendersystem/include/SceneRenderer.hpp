#ifndef _SceneRenderer_H_
#define _SceneRenderer_H_
#pragma 
#include "rendersystem/include/SceneView.hpp"
namespace Air
{
	class CommandListImmediate;

	class ViewInfo;
	class Scene;
	class SceneRenderer
	{
	public:
		Scene* mScene;
		SceneViewFamily mViewFamily;
		std::vector<ViewInfo> mViews;


	public:
		virtual void render(CommandListImmediate& cmdList) = 0;

	protected:
		void onStartFrame(CommandListImmediate& cmdList);
	};


	class ViewInfo : public SceneView
	{
	public:
		void initHardwareResource();
	};

}


#endif
