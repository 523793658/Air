#ifndef _SceneRenderer_H_
#define _SceneRenderer_H_
#pragma 
namespace Air
{
	class CommandListImmediate;
	class ViewInfo;
	class SceneRenderer
	{
	public:
		std::vector<ViewInfo> mViews;


	public:
		virtual void render(CommandListImmediate& cmdList) = 0;


	};
}


#endif
