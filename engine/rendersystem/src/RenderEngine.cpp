#include "Engine.h"
#include "basic/include/Math.hpp"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderLayout.hpp"


#include "rendersystem/include/RenderEngine.hpp"
namespace Air
{
	void RenderEngine::checkConfig(RenderSettings& settings)
	{

	}

	RenderDeviceCaps const & RenderEngine::getDeviceCaps() const
	{
		return mCaps;
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
		mPPRenderLayout = renderFactory.MakeRenderLayout();
		mPPRenderLayout->setTopologyType(RenderLayout::TT_TriangleList);
		Vector2 pos[] = {
			Vector2(-1, 1)
		};
		XMFLOAT2 x(1, 2);
		x *= x;

// 		float2 pos[] = 
// 		{
// 			float2(-1, +1);
// 		}
		
	}
}