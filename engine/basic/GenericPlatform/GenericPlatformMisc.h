#ifndef _GenericPlatformMisc_H_
#define _GenericPlatformMisc_H_
#pragma once
namespace Air
{
	struct GenericPlatformMisc
	{
		static bool getDiskTotalAndFreeSpace(const wchar_t*, uint64_t & totalNumberOfBytes, uint64_t& numberOfFreeBytes);
	};
}
#endif
