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

	class ResLoadingDesc;
	typedef std::shared_ptr<ResLoadingDesc> ResLoadingDescPtr;
	class Camera;
	typedef std::shared_ptr<Camera> CameraPtr;
	class FrameBuffer;
	typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;
	class RenderLayout;
	typedef std::shared_ptr<RenderLayout> RenderLayoutPtr;
	class RenderEngine;
	class GraphicsBuffer;
	typedef std::shared_ptr<GraphicsBuffer> GraphicsBufferPtr;
	class RenderView;
	typedef std::shared_ptr<RenderView> RenderViewPtr;




	struct Viewport;
	typedef std::shared_ptr<Viewport> ViewportPtr;

	template<typename T>
	class Color_T;
	typedef Color_T<float> Color;



}




#endif