#ifndef _Context_H_
#define _Context_H_
#include "Context.h"
#include "core/include/PreDeclare.h"
#include "basic/include/DllLoader.h"
#include "rendersystem/include/RenderSettings.hpp"

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
		std::string mShaderPath;

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


	

		thread_pool& getThreadPool()
		{
			return *mGTPInstance;
		}
	protected:
		RenderFactory& getRenderFactoryInstance();
		InputFactory& getInputFactoryInstance();
		SceneManager& getSceneManangerInstance();
	public:
		friend class SingletonManager;

	private: 
		void destoryAll();
	private:
		static std::unique_ptr<Engine> mEngineInstance;
		ContextCfg mCfg;

		App3DFramework* mApp;
		std::unique_ptr<SceneManager> mSceneMgr;
		std::unique_ptr<RenderFactory> mRenderFactory;
		std::unique_ptr<InputFactory> mInputFactory;
		std::unordered_map<std::string, std::unique_ptr<ResourcePlugin>> mResourcePlugins;

		DllLoader mRenderLoader;
		DllLoader mSceneManagerLoader;
		DllLoader mInputLoader;

		std::unordered_map<std::string, DllLoader> mResourceLoaders;

		//�̳߳�
		std::unique_ptr<thread_pool> mGTPInstance;
	};
}

#endif