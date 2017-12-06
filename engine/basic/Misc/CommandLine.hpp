#ifndef _CommandLine_H_
#define _CommandLine_H_
#pragma once
#include "HAL/CoreTypes.h"
#include "basic/include/STLVector.hpp"

struct CommandLine 
{
	static const uint32_t mMaxCommandLineSize = 16384;

	static const std::string & get();

	static const std::string & getForLogging();

	static const std::string & getOriginal();

	static const std::string & getOriginalForLogging();

	static bool isInitalized();

	static bool set(std::string const & newCommandLine);

	static void append(std::string const & appandString);

	static void addToSubprocessCommandline(std::string const & param);

	static const std::string & getSubprocessCommandline();

	static const std::string & removeExeName(std::string const & cmlLine);

	static void parse(std::string const & cmdLine, std::vector < std::string>& tokens, std::vector<std::string>& switches);
#define whitelistCommandLines()
	static bool mIsInitialized;

	static std::string mCmdLine[mMaxCommandLineSize];

	static std::string mOriginalCmdLine[mMaxCommandLineSize];

	static std::string mLoggingCmdLine[mMaxCommandLineSize];

	static std::string mLoggingOriginalCmdLine[mMaxCommandLineSize];
};

#endif
