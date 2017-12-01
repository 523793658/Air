#ifndef _WindowsPlatform_H_
#define _WindowsPlatform_H_
#pragma once
#include <stdint.h>
#include <algorithm>
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

#if		_MSC_VER >= 1910
#define		AIR_COMPILER_VERSION 141
#elif	_MSC_VER >= 1900
#define		AIR_COMPILER_VERSION 140
#elif	_MSC_VER >= 1800
#define		AIR_COMPILER_VERSION 120
#else
#error	"Unsupported compiler version"
#endif

#define FORCEINLINE		__forceinline
#define FORCENOINLINE	__declspec(noinline)

#if !defined(__clang__) || defined(_MSC_VER)
#define ASSUME(expr)	__assume(expr)
#endif

#if AIR_COMPILER_VERSION >= 141
#define KLAYGE_CXX17_LIBRARY_STRING_VIEW_SUPPORT 1
#endif

#	pragma warning(disable: 4251) // STL classes are not dllexport.
#	pragma warning(disable: 4275) // Derived from non dllexport classes.
#	pragma warning(disable: 4503) // Some decorated name in boost are very 


#ifdef AIR_CXX11_CORE_NOEXCEPT_SUPPORT
#	define AIR_NOEXCEPT noexcept
#	define AIR_NOEXCPET_IF(predicate) noexcept((predicate))
#	define AIR_NOEXCPET_EXPR(expression) noexcept((expression))
#else
#	define	AIR_NOEXCEPT throw()
#	define	AIR_NOEXCPET_IF(predicate)
#	define	KLAYGE_NOEXCEPT_EXPR(expression) false
#endif

#

#ifdef AIR_CPP11_CORE_CONSTEXPR_SUPPORT
#	define AIR_CONSTEXPR	constexpr
#else
#	define AIR_CONSTEXPR
#endif

#if AIR_COMPILER_VERSION >= 140
#	define	AIR_CPP11_CORE_CONSTEXPR_SUPPORT			1
#	define	AIR_CXX11_CORE_NOEXCEPT_SUPPORT				1
#	define	AIR_TS_LIBRARY_FILESYSTEM_V3_SUPPORT		1
#else
#	define	AIR_TS_LIBRARY_FILESYSTEM_V2_SUPPORT		1
#endif

#define PLATFORM_HAS_64BIT_ATOMICS					(WINVER >= 0x600)

#define PLATFORM_HAS_128BIT_ATOMICS					(!HACK_HEADER_GENERATOR && PLATFORM_64BITS && (WINVER >= 0x602))


#endif
