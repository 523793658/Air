
#include "Engine.h"

#include <basic/include/Util.h>



#include "app/include/Window.hpp"
#include "Camera.hpp"

#include "rendersystem/include/RenderFactory.h"

#include "app/include/App3D.hpp"


namespace Air
{
	App3DFramework::App3DFramework(std::string const & name)
		:mName(name), mTotalNumFrames(0), mFps(0), mAccumulateTime(0), mNumFrames(0),
		mAppTime(0), mFrameTime(0)
	{
		Context::getInstance().setAppInstance(*this);
		ContextCfg cfg = Context::getInstance().getConfig();
		mMainWnd = this->makeWindow(mName, cfg.mGraphicsCfg);
#ifndef AIR_PLATFORM_WINDOWS_RUNTIME
		cfg.mGraphicsCfg.left = mMainWnd->getLeft();
		cfg.mGraphicsCfg.top = mMainWnd->getTop();
		cfg.mGraphicsCfg.width = mMainWnd->getWidth();
		cfg.mGraphicsCfg.height = mMainWnd->getHeight();
		Context::getInstance().setConfig(cfg);
#endif // !AIR_PLATFORM_WINDOWS_RUNTIME
	}

	App3DFramework::App3DFramework(std::string const &name, void* native_wnd)
	{
		Context::getInstance().setAppInstance(*this);
		ContextCfg cfg = Context::getInstance().getConfig();
		mMainWnd = this->makeWindow(mName, cfg.mGraphicsCfg, native_wnd);
#ifndef AIR_PLATFORM_WINDOWS_RUNTIME
		cfg.mGraphicsCfg.left = mMainWnd->getLeft();
		cfg.mGraphicsCfg.top = mMainWnd->getTop();
		cfg.mGraphicsCfg.width = mMainWnd->getWidth();
		cfg.mGraphicsCfg.height = mMainWnd->getHeight();
		Context::getInstance().setConfig(cfg);
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
		ContextCfg cfg = Context::getInstance().getConfig();
		Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance().createRenderWindow(mName, cfg.mGraphicsCfg);

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
			}


			this->onDestroy();
		}
	}


	void App3DFramework::onDestroy()
	{

	}
}