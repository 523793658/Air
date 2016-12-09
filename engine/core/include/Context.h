#ifndef _Context_H_
#define _Context_H_
#include "core/include/PreDeclare.h"
#include <string>
#include <boost/assert.hpp>
#include "rendersystem/include/RenderSettings.hpp"
#include "basic/include/DllLoader.h"


namespace Air
{
	struct RenderSettings;

	struct ContextCfg
	{
		std::string mRenderFactoryName;
		std::string mSceneManagerName;


		RenderSettings mGraphicsCfg;
		bool mDeferredRendering;
		bool mPerfProfiler;
		bool mLocationSensor;
	};

	class AIR_CORE_API Context
	{
	public:
		Context();
		~Context();

		static Context& getInstance();
		static void destroy();

		RenderFactory& getRenderFactoryInstance();

		void loadCfg(std::string const & cfg_file);
		void saveCfg(std::string const & cfg_file);

		void setConfig(ContextCfg const& cfg);
		ContextCfg const& getConfig() const;

		void loadRenderFactory(std::string const &rf_name);
		void loadSceneManager(std::string const & sm_name);

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
		static std::unique_ptr<Context> mContextInstance;
		ContextCfg mCfg;

		App3DFramework* mApp;
		std::unique_ptr<SceneManager> mSceneMgr;
		std::unique_ptr<RenderFactory> mRenderFactory;

		DllLoader mRenderLoader;
		DllLoader mSceneManagerLoader;

		//�̳߳�
		std::unique_ptr<thread_pool> mGTPInstance;
	};
}

#endif