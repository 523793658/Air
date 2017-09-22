#include "boost/assert.hpp"




#include "Engine.h"
#include "basic/include/Util.h"
#include "basic/include/Math.hpp"
#include "basic/include/Log.hpp"
#include "basic/include/Thread.h"
#include "rendersystem/include/RenderFactory.h"
#include "scene_manager/include/SceneManager.hpp"
#include "basic/include/Thread.h"
#include "ResLoader.h"
#include "core/include/ResourcePlugin.h"

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

#define AIR_DLL_PREFIX DLL_PREFIX AIR_STRINGIZE(AIR_NAME)

namespace
{
	std::mutex singleton_mutex;
}

namespace Air
{
	std::unique_ptr<Engine> Engine::mEngineInstance;
	typedef void(*makeRenderFactoryFunc)(std::unique_ptr<RenderFactory>& ptr);
	typedef void(*makeSceneManagerFunc)(std::unique_ptr<SceneManager>& ptr);
	typedef void(*makeResourcePluginFunc)(std::unique_ptr<ResourcePlugin>& ptr);



	Engine::Engine()
		: mApp(nullptr)
	{
		//∫Û–¯∂¡≈‰÷√
		mCfg.mGraphicsCfg.full_screen = false;
		mCfg.mGraphicsCfg.height = 900;
		mCfg.mGraphicsCfg.hide_win = false;
		mCfg.mGraphicsCfg.left = 100;
		mCfg.mGraphicsCfg.top = 100;
		mCfg.mGraphicsCfg.width = 1440;
		mCfg.mGraphicsCfg.mColorFmt = EF_ARGB8;
		mCfg.mGraphicsCfg.mDepthStencilFmt = EF_D16;
		mCfg.mGraphicsCfg.mSampleCount = 1;
		mCfg.mGraphicsCfg.mSampleQuality = 0;
		mCfg.mGraphicsCfg.mSyncInterval = 0;
		mCfg.mGraphicsCfg.mMotionFrames = 0;

		mCfg.mRenderFactoryName = "D3D11";
		mCfg.mSceneManagerName = "OCTree";

		mCfg.mAssetsPlugins.push_back(AssetsPluginCfg("fbxPlugin", "fbx"));

		




		mGTPInstance = MakeUniquePtr<thread_pool>(1, 16);
	}

	Engine::~Engine()
	{

	}

	void Engine::destoryAll()
	{
		mSceneMgr.reset();
		mRenderFactory.reset();
		mApp = nullptr;
	}

	Engine& Engine::getInstance()
	{
		if (!mEngineInstance)
		{
			std::lock_guard<std::mutex> lock(singleton_mutex);
			if (!mEngineInstance)
			{
				mEngineInstance = MakeUniquePtr<Engine>();
			}
		}
		return *mEngineInstance;
	}

	void Engine::destroy()
	{
		std::lock_guard<std::mutex> lock(singleton_mutex);
		if (mEngineInstance)
		{
			mEngineInstance->destoryAll();
			mEngineInstance.reset();
		}

	}

	void Engine::loadCfg(std::string const & cfg_file)
	{
// 		int width = 800;
// 		int height = 600;
// 		ElementFormat colorFmt = EF_ARGB8;
// 		ElementFormat depthStencilFmt = EF_D16;
// 		int sampleCount = 1;
// 		int sampleQuality = 0;
// 		bool fullScreen = false;
// 		int syncInterval = 0;
// 		int motionFrames = 0;
// 		bool hdr = false;
// 
// 		std::string rfName = "D3D11";
// 		std::string smName;

		//ResIdentifierPtr file = ResLoader::getInstance().open(cfg_file);
		//if (file)
		//{
		//	//XMLDocument cfg_doc;
		//}
	}

	void Engine::setConfig(ContextCfg const & cfg)
	{
		mCfg = cfg;
		
	}

	ContextCfg const& Engine::getConfig() const
	{
		return mCfg;
	}


	void Engine::setAppInstance(App3DFramework& app)
	{
		mApp = &app;
	}

	App3DFramework& Engine::getAppInstance()
	{
		BOOST_ASSERT(mApp);
		AIR_ASSUME(mApp);
		return *mApp;
	}

	void Engine::loadResourcePlugins(std::vector<AssetsPluginCfg> const &rp_cfg)
	{
		for (const AssetsPluginCfg& acf : rp_cfg)
		{
			std::string resource_plugins_path = ResLoader::getInstance().locate("resource_plugin");
			std::string full_name = AIR_DLL_PREFIX"ResourcePlugins" + acf.mName + DLL_SUFFIX;
			std::string path = resource_plugins_path + "/" + full_name;
			DllLoader loader;
			loader.load(ResLoader::getInstance().locate(path));
			makeResourcePluginFunc mrpf = (makeResourcePluginFunc)loader.getProcAddress("makeResourcePlugin");
			if (mrpf != nullptr)
			{
				mResourcePlugins.emplace(acf.mName, std::unique_ptr<ResourcePlugin>());
				mrpf(mResourcePlugins[acf.mName]);
				mResourceLoaders.emplace(acf.mName, loader);
			}
			else
			{
				logError("loading %s failed", path.c_str());
				loader.free();
			}
		}
	}

	SceneManager& Engine::getSceneManangerInstance()
	{
		if (!mSceneMgr)
		{
			std::lock_guard<std::mutex> lock(singleton_mutex);
			if (!mSceneMgr)
			{
				this->loadSceneManager(mCfg.mSceneManagerName);
			}
		}
		return *mSceneMgr;
	}

	void Engine::loadSceneManager(std::string const & sm_name)
	{
		mSceneMgr.reset();
		mSceneManagerLoader.free();
		std::string scene_manager_path = ResLoader::getInstance().locate("sceneManager");
		std::string full_name = AIR_DLL_PREFIX"SceneManager" + sm_name + DLL_SUFFIX;
		std::string path = scene_manager_path + "/" + full_name;
		mSceneManagerLoader.load(ResLoader::getInstance().locate(path));
		makeSceneManagerFunc msf = (makeSceneManagerFunc)mSceneManagerLoader.getProcAddress("makeSceneManager");
		if (msf != nullptr)
		{
			msf(mSceneMgr);
		}
		else
		{
			logError("loading %s failed", path.c_str());
			mSceneManagerLoader.free();
		}
	}

	void Engine::loadRenderFactory(std::string const &rf_name)
	{
		mRenderFactory.reset();
		mRenderLoader.free();
		std::string render_path = ResLoader::getInstance().locate("render");
		std::string fn = AIR_DLL_PREFIX"RenderEngine" + rf_name + DLL_SUFFIX;
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


	RenderFactory& Engine::getRenderFactoryInstance()
	{
		if (!mRenderFactory)
		{
			std::lock_guard<std::mutex> lock(singleton_mutex);
			if (!mRenderFactory)
			{
				this->loadRenderFactory(mCfg.mRenderFactoryName);
			}
		}
		return *mRenderFactory;
	}
}




#include "Context.h"
