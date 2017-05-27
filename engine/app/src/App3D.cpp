
#include "Engine.h"

#include <basic/include/Util.h>



#include "app/include/Window.hpp"
#include "Camera.hpp"

#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderEngine.hpp"
#include "rendersystem/include/Viewport.hpp"
#include "rendersystem/include/FrameBuffer.hpp"

#include "app/include/App3D.hpp"


namespace Air
{
	App3DFramework::App3DFramework(std::string const & name)
		:mName(name), mTotalNumFrames(0), mFps(0), mAccumulateTime(0), mNumFrames(0),
		mAppTime(0), mFrameTime(0)
	{
		Engine::getInstance().setAppInstance(*this);
		ContextCfg cfg = Engine::getInstance().getConfig();
		mMainWnd = this->makeWindow(mName, cfg.mGraphicsCfg);
#ifndef AIR_PLATFORM_WINDOWS_RUNTIME
		cfg.mGraphicsCfg.left = mMainWnd->getLeft();
		cfg.mGraphicsCfg.top = mMainWnd->getTop();
		cfg.mGraphicsCfg.width = mMainWnd->getWidth();
		cfg.mGraphicsCfg.height = mMainWnd->getHeight();
		Engine::getInstance().setConfig(cfg);
#endif // !AIR_PLATFORM_WINDOWS_RUNTIME
	}

	App3DFramework::App3DFramework(std::string const &name, void* native_wnd)
	{
		Engine::getInstance().setAppInstance(*this);
		ContextCfg cfg = Engine::getInstance().getConfig();
		mMainWnd = this->makeWindow(mName, cfg.mGraphicsCfg, native_wnd);
#ifndef AIR_PLATFORM_WINDOWS_RUNTIME
		cfg.mGraphicsCfg.left = mMainWnd->getLeft();
		cfg.mGraphicsCfg.top = mMainWnd->getTop();
		cfg.mGraphicsCfg.width = mMainWnd->getWidth();
		cfg.mGraphicsCfg.height = mMainWnd->getHeight();
		Engine::getInstance().setConfig(cfg);
#endif // !AIR_PLATFORM_WINDOWS_RUNTIME

	}
	App3DFramework::~App3DFramework()
	{
		//this->destroy();
	}

#ifdef AIR_PLATFORM_WINDOWS_RUNTIME
#else
	void App3DFramework::create()
	{
#endif
		ContextCfg cfg = Engine::getInstance().getConfig();
		RenderFactory& factory = Engine::getInstance().getRenderFactoryInstance();
		factory.getRenderEngineInstance().createRenderWindow(mName, cfg.mGraphicsCfg);
		Engine::getInstance().setConfig(cfg);
		this->onCreate();
	}

	WindowPtr App3DFramework::makeWindow(std::string const & name, RenderSettings const & settings)
	{
		return MakeSharedPtr<Window>(name, settings);
	}

	WindowPtr App3DFramework::makeWindow(std::string const & name, RenderSettings const & settings, void* native_wmd)
	{
		return MakeSharedPtr<Window>(name, settings, native_wmd);
	}

	void App3DFramework::run()
	{
		RenderEngine& engine = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		bool gotMsg;
		MSG msg;
		::PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE);
		while(WM_QUIT != msg.message)
		{
			if (mMainWnd->getActive())
			{
				gotMsg = (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0);
			}
			else
			{
				gotMsg = (::GetMessage(&msg, nullptr, 0, 0) != 0);
			}
			if (gotMsg)
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				//¸üÐÂäÖÈ¾
				engine.refresh();
			}


			this->onDestroy();
		}
	}
	Camera const & App3DFramework::getActiveCamera() const
	{
		RenderEngine& re = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		CameraPtr const & camera = re.getCurrentFrameBuffer()->getViewport()->mCamera;
		BOOST_ASSERT(camera);
		return *camera;
	}

	Camera& App3DFramework::getActiveCamera()
	{
		RenderEngine& re = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		CameraPtr const & camera = re.getCurrentFrameBuffer()->getViewport()->mCamera;
		BOOST_ASSERT(camera);
		return *camera;
	}

	float App3DFramework::getAppTime() const
	{
		return mAppTime;
	}

	float App3DFramework::getFrameTime() const
	{
		return mFrameTime;
	}


	void App3DFramework::onDestroy()
	{

	}

	uint32_t App3DFramework::update(uint32_t pass)
	{
		if (0 == pass)
		{
			//this->updates
		}
		return this->doUpdate(pass);
	}

	void App3DFramework::LookAt(float3 const & eye, float3 const & lookAt)
	{
		this->getActiveCamera().setViewParams(eye, lookAt);
	}
	void App3DFramework::LookAt(float3 const & eye, float3 const & lookAt, float3 const & up)
	{
		this->getActiveCamera().setViewParams(eye, lookAt, up);
	}
	void App3DFramework::Proj(float nearPlane, float farPlane)
	{
		BOOST_ASSERT(nearPlane != 0);
		BOOST_ASSERT(farPlane != 0);
		RenderEngine& re = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		FrameBuffer& fb = *re.getCurrentFrameBuffer();

		this->getActiveCamera().setProjParams(re.getDefaultFov(), static_cast<float>(fb.getWidth()) / fb.getHeight(), nearPlane, farPlane);
	}
}