#include "Engine.h"
#include "basic/include/Math.hpp"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderLayout.hpp"
#include "app/include/App3D.hpp"
#include "app/include/Window.hpp"
#include "rendersystem/include/RenderStateObject.hpp"
#include "scene_manager/include/SceneManager.hpp"
#include "rendersystem/include/PostProcess.hpp"

#include "rendersystem/include/RenderEngine.hpp"
namespace Air
{
	RenderEngine::RenderEngine() 
		: mNumPrimitivesJustRendered(0), mNumVerticesJustRendered(0),
		mNumDrawsJustCalled(0), mNumDispatchesJustCalled(0),
		mDefaultFov(PI / 4), mDefaultRenderWidthScale(1),
		mDefaultRenderHeightScale(1), mStereoMethod(STM_None),
		mStereoSeparation(0), mForceLineMode(false)
	{

	}
	RenderEngine::~RenderEngine()
	{

	}

	RenderEnvironment& RenderEngine::getRenderEnvironment()
	{
		return mRenderEnvironment;
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
		uint32_t const oldScreenWidth = mScreenFrameBuffer->getWidth();
		uint32_t const oldScreenHeight = mScreenFrameBuffer->getHeight();
		uint32_t const newScreenWidth = width;
		uint32_t const newScreenHeight = height;
		uint32_t const newRenderWidth = static_cast<uint32_t>(newScreenWidth * mDefaultRenderWidthScale + 0.5f);
		uint32_t const newRenderHeight = static_cast<uint32_t>(newScreenHeight * mDefaultRenderHeightScale + 0.5f);
// 		if ((oldScreenWidth != newScreenWidth) || (oldScreenHeight != newScreenHeight))
// 		{
// 			this->do
// 		}

	}

	float RenderEngine::getDefaultFov() const
	{
		return mDefaultFov;
	}
	void RenderEngine::setDefaultFov(float fov)
	{
		mDefaultFov = fov;
	}

	FrameBufferPtr const & RenderEngine::getScreenFrameBuffer() const
	{
		return mScreenFrameBuffer;
	}
	FrameBufferPtr const & RenderEngine::getOverlayFrameBuffer() const
	{
		return mOverlayFrameBuffer;
	}

	FrameBufferPtr const & RenderEngine::getCurrentFrameBuffer() const
	{
		return mCurrenFrameBuffer;
	}

	void RenderEngine::refresh()
	{
		Engine::getInstance().getSceneManangerInstance().update();
	}

	void RenderEngine::destroyRenderWindow()
	{

	}

	void RenderEngine::adjustProjectionMatrix(float4x4&)
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
		return mDefaultFrameBuffer;
	}

	void RenderEngine::setDefaultFrameBuffer(FrameBufferPtr frameBuffer)
	{
		mDefaultFrameBuffer = frameBuffer;
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

	void RenderEngine::postProcess(bool skip)
	{
		if (mPostProcessChain)
		{
			mPostProcessChain->update();
		}


	}

	void RenderEngine::updateGPUTimestampsFrequency()
	{

	}
	void RenderEngine::beginFrame()
	{
		mRenderEnvironment.update();
		this->bindFrameBuffer(mDefaultFrameBuffer);
	}
	void RenderEngine::beginPass()
	{

	}

	void RenderEngine::render(RenderEffect const &effect, RenderTechnique const & tech, RenderLayout const & rl)
	{
		this->doRender(effect, tech, rl);
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
		uint32_t const screen_width = mScreenFrameBuffer->getWidth();
		uint32_t const screen_height = mScreenFrameBuffer->getHeight();
		float const screen_aspect = static_cast<float>(screen_width) / screen_height;
		if (!MathLib::equal(screen_aspect, static_cast<float>(settings.width) / settings.height))
		{
			settings.width = static_cast<uint32_t>(settings.height * screen_aspect + 0.5f);
		}
		RenderFactory& renderFactory = Engine::getInstance().getRenderFactoryInstance();

		uint32_t const render_width = static_cast<uint32_t>(settings.width * mDefaultRenderWidthScale + 0.5);
		uint32_t const render_height = static_cast<uint32_t>(settings.height * mDefaultRenderHeightScale + 0.5);

		bool need_resize = false;
		float2 posScale;

		if (!settings.hide_win)
		{
			//need_resize = ((render_width != screen_width) || (render_height != screen_height));
			//TODO 缩放后处理，将背缓转移到屏幕缓冲
// 			float const scale_x = static_cast<float>(screen_width) / render_width;
// 			float const scale_y = static_cast<float>(screen_height) / render_height;
// 
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
		
		//后处理相关数据


		mDefaultFrameBuffer = mScreenFrameBuffer;
		this->bindFrameBuffer(mDefaultFrameBuffer);
		mPostProcessChain = MakeSharedPtr<PostProcessChain>();
		mPostProcessChain->loadCfg(settings.mPostProcessCfgPath);
	}

	void RenderEngine::setStateObject(RenderStateObjectPtr const & rs_obj)
	{
		if (mCurrentLineRenderStateObject != rs_obj)
		{
			if (mForceLineMode)
			{
				auto rs_desc = rs_obj->getRasterizerStateDesc();
				auto const & dds_desc = rs_obj->getDepthStencilStateDesc();
				auto const & bs_desc = rs_obj->getBlendStateDesc();

				rs_desc.mPolygonMode = PM_Line;
				mCurrentLineRenderStateObject = Engine::getInstance().getRenderFactoryInstance().makeRenderStateObject(rs_desc, dds_desc, bs_desc);
				mCurrentLineRenderStateObject->active();
			}
			else
			{
				rs_obj->active();
			}
			mCurrentRenderStateObject = rs_obj;
		}
	}

}