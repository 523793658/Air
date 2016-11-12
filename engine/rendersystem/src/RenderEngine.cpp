#include "Engine.h"
#include "basic/include/Math.hpp"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderLayout.hpp"


#include "rendersystem/include/RenderEngine.hpp"
namespace Air
{
	RenderEngine::RenderEngine()
		:mDefaultRenderWidthScale(1), mDefaultRenderHeightScale(1),
		mStereoSeparation(0),
		mFBStage(0)
	{

	}
	RenderEngine::~RenderEngine()
	{

	}

	uint32_t RenderEngine::getNumPrimitivesJustRendered()
	{
		return mNumPrimitivesJustRendered;
	}
	uint32_t RenderEngine::getNumVerticesJustRendered()
	{
		return mNumVerticesJustRendered;
	}
	uint32_t RenderEngine::getNumDrawsJustCalled()
	{
		return mNumDrawsJustCalled;
	}

	uint32_t RenderEngine::getNumDispatchesJustCalled()
	{
		return mNumDispatchesJustCalled;
	}

	void RenderEngine::resize(uint32_t width, uint32_t height)
	{
		uint32_t const oldScreenWidth = mDefaultFrameBuffers[3]->getWidth();
		uint32_t const oldScreenHeight = mDefaultFrameBuffers[3]->getHeight();
		uint32_t const newScreenWidth = width;
		uint32_t const newScreenHeight = height;
		uint32_t const newRenderWidth = static_cast<uint32_t>(newScreenWidth * mDefaultRenderWidthScale + 0.5f);
		uint32_t const newRenderHeight = static_cast<uint32_t>(newScreenHeight * mDefaultRenderHeightScale + 0.5f);
// 		if ((oldScreenWidth != newScreenWidth) || (oldScreenHeight != newScreenHeight))
// 		{
// 			this->do
// 		}

	}

	void RenderEngine::destroyRenderWindow()
	{

	}


	void RenderEngine::checkConfig(RenderSettings& settings)
	{

	}

	RenderDeviceCaps const & RenderEngine::getDeviceCaps() const
	{
		return mCaps;
	}

	FrameBufferPtr const & RenderEngine::getDefaultFrameBuffer() const
	{
		return mDefaultFrameBuffers[mFBStage];
	}

	void RenderEngine::bindFrameBuffer(FrameBufferPtr const &fb)
	{
		FrameBufferPtr newFB;
		if (fb)
		{
			newFB = fb;
		}
		else
		{
			newFB = this->getDefaultFrameBuffer();
		}
		if ((fb != newFB) || (fb && fb->isDirty()))
		{
			if (mCurrenFrameBuffer)
			{
				mCurrenFrameBuffer->onUnbind();
			}
			mCurrenFrameBuffer = newFB;
			mCurrenFrameBuffer->onBind();
			this->doBindFrameBuffer(mCurrenFrameBuffer);
		}
	}

	void RenderEngine::updateGPUTimestampsFrequency()
	{

	}
	void RenderEngine::beginFrame()
	{
		this->bindFrameBuffer(mDefaultFrameBuffers[0]);
	}
	void RenderEngine::beginPass()
	{

	}

	void RenderEngine::endPass()
	{

	}
	void RenderEngine::endFrame()
	{

	}
	void RenderEngine::suspend()
	{
		this->doSuspend();
	}
	void RenderEngine::resume()
	{
		this->doResume();
	}

	void RenderEngine::createRenderWindow(std::string const & name, RenderSettings& settings)
	{
		if (settings.mStereoMethod != STM_OculusVR)
		{
			mStereoSeparation = settings.mStereoSeparation;
		}
		this->doCreateRenderWindow(name, settings);
		this->checkConfig(settings);
		RenderDeviceCaps const & caps = this->getDeviceCaps();
		mScreenFrameBuffer = mCurrenFrameBuffer;
		uint32_t const screen_width = mScreenFrameBuffer->getWidth();
		uint32_t const screen_height = mScreenFrameBuffer->getHeight();
		float const screen_aspect = static_cast<float>(screen_width) / screen_height;
		if (!MathLib::equal(screen_aspect, static_cast<float>(settings.width) / settings.height))
		{
			settings.width = static_cast<uint32_t>(settings.height * screen_aspect + 0.5f);
		}
		RenderFactory& renderFactory = Context::getInstance().getRenderFactoryInstance();

		uint32_t const render_width = static_cast<uint32_t>(settings.width * mDefaultRenderWidthScale + 0.5);
		uint32_t const render_height = static_cast<uint32_t>(settings.height * mDefaultRenderHeightScale + 0.5);

		bool need_resize = false;
		if (!settings.hide_win)
		{
			//need_resize = ((render_width != screen_width) || (render_height != screen_height));
			//TODO 缩放后处理，将背缓转移到屏幕缓冲
// 			float const scale_x = static_cast<float>(screen_width) / render_width;
// 			float const scale_y = static_cast<float>(screen_height) / render_height;
// 
// 			Float2 posScale;
// 			if (scale_x < scale_y)
// 			{
// 				posScale.x = 1;
// 				posScale.y = (scale_x * render_height) / screen_height;
// 			}
// 			else
// 			{
// 				posScale.x = (scale_y * render_width) / screen_width;
// 			}
// 
// 			for (size_t i = 0; i < 2; ++i)
// 			{
// 			}
		}


		//mPPRenderLayout = renderFactory.MakeRenderLayout();
		//mPPRenderLayout->setTopologyType(RenderLayout::TT_TriangleList);
		
// 		Float2 pos[] =
// 		{
// 			Float2(-1, 1),
// 			Float2(1, 1),
// 			Float2(-1, -1),
// 			Float2(1, -1)
// 		};
		//后处理相关数据


		for (int i = 0; i < 4; ++i)
		{
			mDefaultFrameBuffers[i] = mScreenFrameBuffer;
		}

		this->bindFrameBuffer(mDefaultFrameBuffers[0]);
	}
}