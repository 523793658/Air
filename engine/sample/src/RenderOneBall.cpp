#include "core/include/Engine.h"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/RenderEngine.hpp"
#include "rendersystem/include/RenderView.hpp"
#include "sample/include/RenderOneBall.hpp"

using namespace Air;

namespace
{
	class SphereObject: public SceneObjectHelper


}

SampleRenderOneBall::SampleRenderOneBall() : App3DFramework("createWindow")
{

}

int SampleMain()
{
	ContextCfg  cfg = Context::getInstance().getConfig();
	SampleRenderOneBall app;
	app.create();
	app.run();
	return 0;
}

void SampleRenderOneBall::onCreate()
{
	mSphere = MakeSharedPtr <
}

uint32_t SampleRenderOneBall::doUpdate(uint32_t pass)
{
	RenderEngine& re = Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
	Color32 c = { 1.0, 1.0, 0.0, 1.0 };
	re.getCurrentFrameBuffer()->clear(FrameBuffer::CBM_Color | FrameBuffer::CBM_Depth | FrameBuffer::CBM_Stencil, c, 1.0, 0);
	return App3DFramework::URV_NeedFlush | App3DFramework::URV_Finished;
}
