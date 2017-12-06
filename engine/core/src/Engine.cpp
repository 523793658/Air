#include "boost/assert.hpp"


#include "Context.h"


#include "basic/include/Util.h"
#include "basic/include/Math.hpp"
#include "basic/include/Log.hpp"
#include "basic/include/Thread.h"
#include "rendersystem/include/RenderFactory.h"
#include "scene_manager/include/SceneManager.hpp"
#include "input_system/include/InputFactory.hpp"
#include "basic/include/Thread.h"
#include "ResLoader.h"
#include "app/include/App3D.hpp"
#include "app/include/Window.hpp"
#include "core/include/ResourcePlugin.h"

#include <memory>
#include <mutex>






#define AIR_DLL_PREFIX DLL_PREFIX AIR_STRINGIZE(AIR_NAME)

namespace
{
	std::mutex singleton_mutex;
}

namespace Air
{
	std::unique_ptr<Engine> Engine::mEngineInstance;
	typedef void(*makeRenderFactoryFunc)(std::unique_ptr<RenderFactory>& ptr);
	typedef void(*makeSceneManagerFunc)(std::unique_ptr<SceneManager>& ptr, AABBox const & worldSize);
	typedef void(*makeResourcePluginFunc)(std::unique_ptr<ResourcePlugin>& ptr);

	typedef void(*makeInputFactoryFunc)(std::unique_ptr<InputFactory>& ptr);



	Engine::Engine()
		: mApp(nullptr)
	{
		//后续读配置
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
		mCfg.mGraphicsCfg.mPostProcessCfgPath = "assets/cfg/postprocess.xml";
		mCfg.mGraphicsCfg.mPipelineConfigPath = "assets/cfg/pipeline.xml";

		mCfg.mRenderFactoryName = "D3D11";
		mCfg.mSceneManagerName = "OCTree";
		mCfg.mInputFactoryName = "Msg";

		mCfg.mAssetsPlugins.push_back(AssetsPluginCfg("fbxPlugin", "fbx"));

		mCfg.mShaderPath = "assets/shader";
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

	void Engine::update()
	{
		if (mApp->getMainWnd()->getActive())
		{
			//事件处理引擎更新
			mInputFactory->getInputEngineInstance().update();

			//更新逻辑
			mApp->update(0);

			//UI更新

			//更新场景
			mSceneMgr->update();

			//渲染引擎更新
			mRenderFactory->getRenderEngineInstance().refresh();
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

	void Engine::loadInputFactory(std::string const & if_name)
	{
		mInputFactory.reset();
#if !(defined(AIR_PLATFORM_ANDROID) || defined(AIR_PLATFORM_IOS))
		mInputLoader.free();
		std::string input_path = ResLoader::getInstance().locate("Input");
		std::string fn = AIR_DLL_PREFIX"InputEngine" + if_name + DLL_SUFFIX;
		std::string path = input_path + "/" + fn;
		mInputLoader.load(ResLoader::getInstance().locate(path));
		makeInputFactoryFunc mif = (makeInputFactoryFunc)mInputLoader.getProcAddress("makeInputFactory");
		if (mif != nullptr)
		{
			mif(mInputFactory);
		}
		else
		{
			logError("Loading %s failed", path.c_str());
			mInputLoader.free();
		}
#else
		AIR_UNUSED(if_name);
		MakeInputFactory(mInputFactory);
#endif

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
			msf(mSceneMgr, AABBox(float3(-1000.0f, -1000.0f, -1000.0f), float3(1000, 1000, 1000)));
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

	InputFactory& Engine::getInputFactoryInstance()
	{
		if (!mInputFactory)
		{
			std::lock_guard<std::mutex> lock(singleton_mutex);
			if (!mInputFactory)
			{
				this->loadInputFactory(mCfg.mInputFactoryName);
			}
		}
		return *mInputFactory;
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
