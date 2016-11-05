#include "core/include/Engine.h"
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
