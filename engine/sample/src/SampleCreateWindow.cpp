#include "core/include/Engine.h"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/RenderEngine.hpp"
#include "rendersystem/include/RenderView.hpp"
#include "sample/include/SampleCreateWindow.hpp"

using namespace Air;

	SampleCreateWindow::SampleCreateWindow() : App3DFramework("createWindow")
	{

	}

	int SampleMain()
	{
		ContextCfg  cfg = Context::getInstance().getConfig();
		SampleCreateWindow app;
		app.create();
		app.run();
		return 0;
	}

	uint32_t SampleCreateWindow::doUpdate(uint32_t pass)
	{
		RenderEngine& re = Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		Color32 c = { 1.0, 1.0, 0.0, 1.0 };
		re.getCurrentFrameBuffer()->clear(FrameBuffer::CBM_Color | FrameBuffer::CBM_Depth | FrameBuffer::CBM_Stencil, c, 1.0, 0);
		return App3DFramework::URV_NeedFlush | App3DFramework::URV_Finished;
	}
