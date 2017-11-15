#ifndef _SceneViewExtension_H_
#define _SceneViewExtension_H_
#pragma once
namespace Air
{

class SceneViewExtension
{
public:
	virtual void postInitView_RenderThread(CommandListImmediate& cmdList, SceneView& inView) {};

	virtual void postInitViewFamily_RenderThread(CommandListImmediate& cmdList, SceneViewFamily& viewFamily) {};
};
}

#endif
