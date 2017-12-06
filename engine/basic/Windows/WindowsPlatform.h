#ifndef _WindowsPlatform_H_
#define _WindowsPlatform_H_
#pragma once
#include "Windows/WindowsHWrapper.h"
#include <stdint.h>
#include <algorithm>
#include "basic/include/Config.h"
struct WindowsPlatformTypes : public GenericPlatformTypes
{
#ifdef _WIN64
	typedef unsigned __int64	SIZE_T;
	typedef __int64				SSIZE_T;
#else
	typedef unsigned long		SIZE_T;
	typedef long				SSIZE_T;
#endif
};

typedef WindowsPlatformTypes PlatformTypes;







#define PLATFORM_DESKTOP		1
#if defined(_WIN64)
#define		PLATFORM_64BITS		1
#else
#define		PLATFORM_64BITS		0
#endif






#	pragma warning(disable: 4251) // STL classes are not dllexport.
#	pragma warning(disable: 4275) // Derived from non dllexport classes.
#	pragma warning(disable: 4503) // Some decorated name in boost are very 


#ifdef AIR_CPP11_CORE_CONSTEXPR_SUPPORT
#	define AIR_CONSTEXPR	constexpr
#else
#	define AIR_CONSTEXPR
#endif

#define PLATFORM_HAS_64BIT_ATOMICS					(WINVER >= 0x600)

#define PLATFORM_HAS_128BIT_ATOMICS					(!HACK_HEADER_GENERATOR && PLATFORM_64BITS && (WINVER >= 0x602))


#endif
