#ifndef _Context_H_
#define _Context_H_
#include "PreDeclare.h"
#include <string>
#include "RenderSettings.h"

#include "DllLoader.h"

namespace Air
{
	struct ContextCfg
	{
		std::string render_factory_name;
		std::string scene_manager_name;

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
		void loadCfg(std::string const & cfg_file);
		void saveCfg(std::string const & cfg_file);

		void setConfig(ContextCfg const& cfg);
		ContextCfg const& getConfig() const;

		void loadRenderFactory(std::string const &rf_name);
		void loadSceneManager(std::string const & sm_name);
	private: 
		void destoryAll();
	private:
		static std::unique_ptr<Context> mContextInstance;
		ContextCfg mCfg;

		App3DFramework* mApp;
		std::unique_ptr<SceneManager> mSceneMgr;
		std::unique_ptr<RenderFactory> mRenderFactory;

		DllLoader mRenderLoader;

		//Ïß³Ì³Ø
		//std::unique_ptr<
	};
}

#endif