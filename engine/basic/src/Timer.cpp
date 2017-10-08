
#include "basic/include/Basic.h"
#include <limits>
#include <chrono>
#include "basic/include/Timer.hpp"

namespace Air
{
	Timer::Timer()
	{
		this->restart();
	}

	void Timer::restart()
	{
		mStartTime = this->getCurrentTime();
	}

	double Timer::getElapsed() const
	{
		return this->getCurrentTime() - mStartTime;
	}

	double Timer::getElapsedMax() const
	{
		return std::chrono::duration<double>::max().count();
	}

	double Timer::getElapsedMin() const
	{
		return std::chrono::duration<double>::min().count();
	}

	double Timer::getCurrentTime() const
	{
		std::chrono::high_resolution_clock::time_point const tp = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::duration<double>>(tp.time_since_epoch()).count();
	}
}