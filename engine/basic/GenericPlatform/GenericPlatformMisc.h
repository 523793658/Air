#ifndef _GenericPlatformMisc_H_
#define _GenericPlatformMisc_H_
#pragma once
#include "basic/include/Basic.h"
namespace Air
{
	struct GenericPlatformMisc
	{
		static bool getDiskTotalAndFreeSpace(std::string const &, uint64_t & totalNumberOfBytes, uint64_t& numberOfFreeBytes);
	};
}
#endif
