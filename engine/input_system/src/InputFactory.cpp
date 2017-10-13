#include "Engine.h"
#include "input_system/include/InputFactory.hpp"

namespace Air
{
	InputEngine& InputFactory::getInputEngineInstance()
	{
		if (!mInputEngine)
		{
			mInputEngine = this->doMakeInputEngine();
		}
		return *mInputEngine;
	}
	void InputFactory::suspend()
	{
		if (mInputEngine)
		{
			mInputEngine->suspend();
		}
		this->doSuspend();
	}
	void InputFactory::resume()
	{
		this->doResume();
		if (mInputEngine)
		{
			mInputEngine->resume();
		}
	}

}