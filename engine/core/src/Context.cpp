#include "Engine.h"
#include "basic/include/Util.h"
#include "basic/include/Math.hpp"
#include "basic/include/Log.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "core/include/SceneManager.hpp"
#include "ResLoader.h"

#include <memory>

#ifdef AIR_PLATFORM_WINDOWS
#include <windows.h>
#if defined(KLAYGE_PLATFORM_WINDOWS_DESKTOP)
#if (_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
#include <VersionHelpers.h>
#endif
#endif
#endif

#include <mutex>


#include "Context.h"

#define AIR_DLL_PREFIX DLL_PREFIX KFL_STRINGIZE(AIR_NAME)

namespace
{
	std::mutex singleton_mutex;
}

namespace Air
{
	std::unique_ptr<Context> Context::mContextInstance;
	typedef void(*makeRenderFactoryFunc)(std::unique_ptr<RenderFactory>& ptr);

	Context::Context()
	{
		mCfg.mGraphicsCfg.full_screen = false;
		mCfg.mGraphicsCfg.height = 900;
		mCfg.mGraphicsCfg.hide_win = false;
		mCfg.mGraphicsCfg.left = 100;
		mCfg.mGraphicsCfg.top = 100;
		mCfg.mGraphicsCfg.width = 1440;
	}

	Context::~Context()
	{

	}

	void Context::destoryAll()
	{
		mSceneMgr.reset();
		mRenderFactory.reset();
		mApp = nullptr;
	}

	Context& Context::getInstance()
	{
		if (!mContextInstance)
		{
			std::lock_guard<std::mutex> lock(singleton_mutex);
			if (!mContextInstance)
			{
				mContextInstance = MakeUniquePtr<Context>();
			}
		}
		return *mContextInstance;
	}

	void Context::destroy()
	{
		std::lock_guard<std::mutex> lock(singleton_mutex);
		if (mContextInstance)
		{
			mContextInstance->destoryAll();
			mContextInstance.reset();
		}

	}

	void Context::loadCfg(std::string const & cfg_file)
	{
		int width = 800;
		int height = 600;
		ElementFormat colorFmt = EF_ARGB8;
		ElementFormat depthStencilFmt = EF_D16;
		int sampleCount = 1;
		int sampleQuality = 0;
		bool fullScreen = false;
		int syncInterval = 0;
		int motionFrames = 0;
		bool hdr = false;

		std::string rfName = "D3D11";
		std::string smName;

		//ResIdentifierPtr file = ResLoader::getInstance().open(cfg_file);
		//if (file)
		//{
		//	//XMLDocument cfg_doc;
		//}
	}

	void Context::setConfig(ContextCfg const & cfg)
	{
		mCfg = cfg;
		
	}

	ContextCfg const& Context::getConfig() const
	{
		return mCfg;
	}


	void Context::setAppInstance(App3DFramework& app)
	{
		mApp = &app;
	}

	void Context::loadRenderFactory(std::string const &rf_name)
	{
		mRenderFactory.reset();
		mRenderLoader.free();
		std::string render_path = ResLoader::getInstance().locate("Render");
		std::string fn = AIR_DLL_PREFIX"_RenderEngine_" + rf_name + DLL_SUFFIX;
		std::string path = render_path + "/" + fn;
		mRenderLoader.load(ResLoader::getInstance().locate(path));
		makeRenderFactoryFunc mrf = (makeRenderFactoryFunc)mRenderLoader.getProcAddress("makeRenderFactory");
		if (mrf != nullptr)
		{
			mrf(mRenderFactory);
		}
		else
		{
			logError("Loading %s failed", path.c_str());
			mRenderLoader.free();
		}
	}


	RenderFactory& Context::getRenderFactoryInstance()
	{
		if (!mRenderFactory)
		{
			std::lock_guard<std::mutex> lock(singleton_mutex);
			if (!mRenderFactory)
			{
				this->loadRenderFactory(mCfg.render_factory_name);
			}
		}
		return *mRenderFactory;
	}
}




#include "Context.h"
