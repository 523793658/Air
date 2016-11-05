#ifndef _Air_Render_engine_H_
#define _Air_Render_engine_H_
#pragma  once;
#include "core/include/PreDeclare.h"
#include "rendersystem/include/RenderDeviceCaps.hpp"
#include "rendersystem/include/RenderSettings.hpp"

namespace Air
{
	class AIR_CORE_API RenderEngine
	{
	public:
		RenderEngine();
		virtual ~RenderEngine();

		void suspend();
		void resume();

		virtual std::wstring const & getName() const = 0;
		virtual bool requiresFlipping() const = 0;

		virtual void beginFrame();
		virtual void beginPass();

		//void render()

		virtual void endPass();
		virtual void endFrame();
		virtual void updateGPUTimestampsFrequency();

		virtual void forceFlush() = 0;
		uint32_t getNumPrimitivesJustRendered();
		uint32_t getNumVerticesJustRendered();
		uint32_t getNumDrawsJustCalled();
		uint32_t getNumDispatchesJustCalled();

		void createRenderWindow(std::string const & name, RenderSettings& settings);
		void destroyRenderWindow();

		//void setStateObjects(Rasterizer)

		void resize(uint32_t width, uint32_t height);
		virtual bool isFullScreen() const = 0;
		virtual void setFullScreen(bool fs) = 0;

		RenderDeviceCaps const & getDeviceCaps() const;

	private:
		virtual void checkConfig(RenderSettings& settings);
		virtual void doCreateRenderWindow(std::string const & name, RenderSettings const & settings) = 0;


	private:
		FrameBufferPtr mCurrenFrameBuffer;
		FrameBufferPtr mScreenFrameBuffer;
		

		float mStereoSeparation;
		RenderDeviceCaps mCaps;

		RenderLayoutPtr mPPRenderLayout;

	};




}

#endif