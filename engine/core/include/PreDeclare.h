#ifndef _PreDeclare_H_
#define _PreDeclare_H_
#include <memory>

namespace Air

{
	struct ConfextCfg;
	class Context;


	class SceneManager;
	class RenderFactory;

	class App3DFramework;
	class Window;
	typedef std::shared_ptr<Window> WindowPtr;


	class Camera;
	typedef std::shared_ptr<Camera> CameraPtr;
	class FrameBuffer;
	typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;
	class RenderLayout;
	typedef std::shared_ptr<RenderLayout> RenderLayoutPtr;
	class RenderEngine;



	struct Viewport;
	typedef std::shared_ptr<Viewport> ViewportPtr;

	template<typename T>
	class Color_T;
	typedef Color_T<float> Color;

}




#endif