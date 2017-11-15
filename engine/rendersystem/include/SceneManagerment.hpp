#ifndef _SceneManagerment_H_
#define _SceneManagerment_H_
#pragma once
namespace Air
{
	class SceneViewStateInterface
	{
	public:
		virtual void onStartFrame(SceneView& view, SceneViewFamily& viewFamily) = 0;
	};
}
#endif
