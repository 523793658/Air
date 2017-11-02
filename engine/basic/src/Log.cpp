#include "basic/include/Basic.h"
#include <cstdarg>
#include <cstdio>
#ifdef AIR_PLATFORM_ANDROID
#include <android/log.h>
#else
#include <iostream>
#include <fstream>
#endif

#include <array>

#include "basic/include/Log.hpp"

#ifdef AIR_DEBUG
#ifndef AIR_PLATFORM_ANDROID
namespace
{
	std::ofstream log_file("Air.log");
}
#endif
#endif


namespace Air
{
	void logInfo(char const * fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		std::array<char, 1024> buffer;
		vsprintf(&buffer[0], fmt, args);
		std::clog << "(INFO) Air: " << &buffer[0] << std::endl;
#ifdef AIR_DEBUG
		log_file << "(INFO) Air: " << &buffer[0] << std::endl;
#endif
		va_end(args);
	}

	void logWarn(char const * fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		std::array<char, 1024 * 10> buffer;
		vsprintf(&buffer[0], fmt, args);
		std::clog << "(WARN) Air: " << &buffer[0] << std::endl;
#ifdef AIR_DEBUG
		log_file << "(WARN) Air: " << &buffer[0] << std::endl;
#endif
		va_end(args);
	}

	void logError(char const * fmt, ...)
	{
		va_list args;
		va_start(args, fmt);


		std::array<char, 1024> buffer;
		vsprintf(&buffer[0], fmt, args);

		std::clog << "(ERROR) KlayGE: " << &buffer[0] << std::endl;
#ifdef AIR_DEBUG
		log_file << "(ERROR) KlayGE: " << &buffer[0] << std::endl;
#endif
		va_end(args);
	}

}