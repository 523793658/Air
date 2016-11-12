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

		void bindFrameBuffer(FrameBufferPtr const &fb);
		FrameBufferPtr const & getDefaultFrameBuffer() const;

	private:
		virtual void checkConfig(RenderSettings& settings);
		virtual void doCreateRenderWindow(std::string const & name, RenderSettings const & settings) = 0;
		virtual void doBindFrameBuffer(FrameBufferPtr const & fb) = 0;
		virtual void doSuspend() = 0;
		virtual void doResume() = 0;

	protected:
		FrameBufferPtr mCurrenFrameBuffer;
		FrameBufferPtr mScreenFrameBuffer;
		FrameBufferPtr mDefaultFrameBuffers[4];

		float mStereoSeparation;
		RenderDeviceCaps mCaps;

		float mDefaultRenderWidthScale;
		float mDefaultRenderHeightScale;
		int mFBStage;

		uint32_t mNumPrimitivesJustRendered;
		uint32_t mNumVerticesJustRendered;
		uint32_t mNumDrawsJustCalled;
		uint32_t mNumDispatchesJustCalled;

		RenderLayoutPtr mPPRenderLayout;

	};




}

#endif