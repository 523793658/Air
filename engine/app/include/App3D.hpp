#ifndef _APP3D_H_
#define _APP3D_H_
#pragma once

#include "core/include/PreDeclare.h"
#include "basic/include/Timer.hpp"
namespace Air
{
	class AIR_CORE_API App3DFramework
	{
		friend class SceneManager;
	public:
		enum UpdateRetValue
		{
			URV_NeedFlush = 1UL << 0,
			URV_Finished = 1UL << 1,
			URV_Overlay = 1UL << 2,
			URV_SkipPostProcess = 1UL << 3,
			URV_OpaqueOnly = 1UL << 4,
			URV_TransparencyBackOnly = 1UL << 5,
			URV_TransparencyFrontOnly = 1UL << 6,
			URV_ReflectionOnly = 1UL << 7,
			URV_SpecialShadingOnly = 1UL << 8,
			URV_SimpleForwardOnly = 1UL << 9
		};

	public:
		explicit App3DFramework(std::string const & name);
		App3DFramework(std::string const & name, void* native_wnd);
		virtual ~App3DFramework();

		void create();
		/*void destroy();
		void suspend();
		void resume();
		void refresh();*/

		std::string const & getName() const
		{
			return mName;
		}

		WindowPtr makeWindow(std::string const & name, RenderSettings const & settings);
		WindowPtr makeWindow(std::string const & name, RenderSettings const & settings, void* native_wnd);
		WindowPtr const & getMainWnd() const
		{
			return mMainWnd;
		}

		virtual bool confirmDevice() const
		{
			return true;
		}

		Camera const & getActiveCamera() const;

 		Camera& getActiveCamera();
// 
// 		uint32_t getTotalNumFrams() const;
// 
// 		float getFps() const;

		float getAppTime() const;

		float getFrameTime() const;
// 		void quit();

		void run();

		//virtual void onResize(uint32_t width, uint32_t height);

	protected:
		void LookAt(float3 const & eye, float3 const & lookAt);
		void LookAt(float3 const & eye, float3 const & lookAt, float3 const & up);
		void Proj(float nearPlane, float farPlane);

	protected:
		uint32_t update(uint32_t pass);
		//void UpdateStats();

	private:
		virtual void onCreate()
		{

		}
		virtual void onDestroy();
		virtual void onSuspend()
		{

		}
		virtual void onResume()
		{

		}

		//virtual void doUpdateOverlay() = 0;
		virtual uint32_t doUpdate(uint32_t pass) = 0;

	protected:
		std::string mName;
		uint32_t mTotalNumFrames;
		float mFps;
		float mAccumulateTime;
		uint32_t mNumFrames;

		Timer mTimer;
		float mAppTime;
		float mFrameTime;
		WindowPtr mMainWnd;
	};
}

#endif