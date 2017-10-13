#ifndef _Context_H_
#define _Context_H_
#include "core/include/PreDeclare.h"
#include <string>
#include "basic/include/Log.hpp"
#include "basic/include/ErrorHanding.hpp"
#include <boost/assert.hpp>
#include "rendersystem/include/RenderSettings.hpp"
#include "basic/include/DllLoader.h"
#include "map"


namespace Air
{
	struct RenderSettings;

	struct  AssetsPluginCfg
	{
	public:
		AssetsPluginCfg(std::string name, std::string format) : mName(name), mFormat(format)
		{

		}
	public:
		std::string mName;
		std::string mFormat;
	};

	struct ContextCfg
	{
		std::string mRenderFactoryName;
		std::string mInputFactoryName;
		std::string mSceneManagerName;


		RenderSettings mGraphicsCfg;
		std::vector<AssetsPluginCfg> mAssetsPlugins;
		bool mDeferredRendering;
		bool mPerfProfiler;
		bool mLocationSensor;
	};

	class AIR_CORE_API Engine
	{
	public:
		Engine();
		~Engine();

		static Engine& getInstance();
		static void destroy();

		void update();

		RenderFactory& getRenderFactoryInstance();
		InputFactory& getInputFactoryInstance();

		

		void loadCfg(std::string const & cfg_file);
		void saveCfg(std::string const & cfg_file);

		void setConfig(ContextCfg const& cfg);
		ContextCfg const& getConfig() const;

		void loadRenderFactory(std::string const &rf_name);
		void loadSceneManager(std::string const & sm_name);
		void loadInputFactory(std::string const & if_name);
		void loadResourcePlugins(std::vector<AssetsPluginCfg> const &rp_cfg);

		void setAppInstance(App3DFramework& app);

		App3DFramework& getAppInstance();
		SceneManager& getSceneManangerInstance();

		thread_pool& getThreadPool()
		{
			return *mGTPInstance;
		}
	private: 
		void destoryAll();
	private:
		static std::unique_ptr<Engine> mEngineInstance;
		ContextCfg mCfg;

		App3DFramework* mApp;
		std::unique_ptr<SceneManager> mSceneMgr;
		std::unique_ptr<RenderFactory> mRenderFactory;
		std::unique_ptr<InputFactory> mInputFactory;
		std::map<std::string, std::unique_ptr<ResourcePlugin>> mResourcePlugins;

		DllLoader mRenderLoader;
		DllLoader mSceneManagerLoader;
		DllLoader mInputLoader;

		std::map<std::string, DllLoader> mResourceLoaders;

		//Ïß³Ì³Ø
		std::unique_ptr<thread_pool> mGTPInstance;
	};
}

#endif