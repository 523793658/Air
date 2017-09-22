#ifndef _Config_H_
#define _Config_H_

#if !defined(__cplusplus)
#error C++ compiler required.
#endif

#if defined(DEBUG) | defined(_DEBUG)
#define AIR_DEBUG
#endif

#define AIR_STRINGIZE(X) AIR_DO_STRINGIZE(X)
#define AIR_DO_STRINGIZE(X) #X

#define AIR_JOIN(X, Y) AIR_DO_JOIN(X, Y)
#define AIR_DO_JOIN(X, Y) AIR_DO_JOIN2(X, Y)
#define AIR_DO_JOIN2(X, Y) X##Y

#if defined(__clang__)
#if __cplusplus < 201402L
#error	"-std=c++14 must be turned on"
#endif

#define AIR_COMPILER_CLANG
#define AIR_COMPILER_NAME	clang

#define CLANG_VERSION	AIR_JOIN(__clang_major__, __clang_minor__)

#if __cplusplus > 201402L
#define	AIR_CXX17_CORE_STATIC_ASSERT_V2_SUPPORT
#endif

#if defined(__APPLE__)

#if CLANG_VERSION >= 61
#define AIR_COMPILER_VERSION CLANG_VERSION
#else
#error "Unsupported compiler version. Please install Apple clang++ 6.1 or up."
#endif

#define AIR_CXX17_LIBRARY_SIZE_AND_MORE_SUPPORT
#define AIR_SYMBOL_EXPORT __attribute__((__visibility__("default")))
#define AIR_SYMBOL_IMPORT
#elif defined(__ANDROID__)
#if CLANG_VERSION >= 36
#define AIR_COMPILER_VERSION CLANG_VERSION
#else
#error "Unsupported compiler version. please install clang++ 3.6 or up."
#endif
#define AIR_TS_LIBRARY_OPTIONAL_SUPPORT
#define AIR_SYMBOL_EXPORT __attribute__((__visibility__("default")))
#define AIR_SYMBOL_IMPORT
#elif defined(__c2__)
#if CLANG_VERSION >= 36
#define AIR_COMPILER_VERSION CLANG_VERSION
#else
#error "Unsupported compiler version. Please Install clang++ 3.6 or up."
#endif
#define AIR_COMPILER_CLANGC2
#define AIR_CXX17_LIBRARY_SIZE_AND_MORE_SUPPORT
#define AIR_TS_LIBRARY_FILESYSTEM_SUPPORT

#define AIR_SYMBOL_EXPORT __declspec(dllexport)
#define AIR_SYMBOL_IMPORT __declspec(dllimport)
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _SCL_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_DEPRECATE
#endif
#else
#error "Clang++ on an unknown platform. Only Apple and Windows are supported."
#endif

#define AIR_ATTRIBUTE_NORETURN __attribute__((noreturn))
#define AIR_BUILTIN_UNREACHABLE __builtin_unreachable()

#elif defined(__GNUC__)
#define AIR_COMPILER_GCC
#include <bits/c++config.h>
#ifdef _GLIBCXX_USE_FLOAT128
#undef _GLIBCXX_USE_FLOAT128
#endif
#ifdef _GLIBCXX_USE_INT128
#undef _GLIBCXX_USE_INT128
#endif
#define GCC_VERSION AIR_JOIN(__GNUC__, __GNUC_MINOR__)
#if GCC_VERSION >= 51
#define  AIR_COMPILER_VERSION GCC_VERSION
#else
#error "Unsupported compiler version, Please install g++ 5.1 or up."
#endif
#if __cplusplus < 201402L
#error "-std=c++14 must be turned on."
#endif
#if !defined(_GLIBCXX_HAS_GTHREADS)
#error "_GLIBCXX_HAS_GTHREADS must be turned on."
#endif
#if AIR_COMPILER_VERSION >= 71
#define AIR_CXX17_LIBRARY_ANY_SUPPORT
#define AIR_CXX17_LIBRARY_OPTIONAL_SUPPORT
#define AIR_CXX17_LIBRARY_STRING_VIEW_SUPPORT
#else
#define AIR_TS_LIBRARY_ANY_SUPPORT
#define AIR_TS_LIBRARY_OPTIONAL_SUPPORT
#endif
#if AIR_COMPILER_VERSION >= 61
#define AIR_CXX17_LIBRARY_SIZE_AND_MORE_SUPPORT
#define AIR_TS_LIBRARY_FILESYSTEM_SUPPORT
#endif
#if AIR_COMPILER_VERSION >=60
#if __cplusplus > 201402L
#define AIR_CXX17_CORE_STATIC_ASSERT_V2_SUPPORT
#endif
#endif
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define AIR_SYMBOL_EXPORT __attribute__((__dllexport__))
#define AIR_SYMBOL_IMPORT __attribute__((__dllimport__))
#else
#define AIR_SYMBOL_EXPORT __attribute__((__visibility__("default")))
#define AIR_SYMBOL_IMPORT
#endif

#define AIR_ATTRIBUTE_NORETURN __attribute__((noreturn))
#define AIR_BUILTIN_UNREACHABLE __builtin_unreachable()


#elif defined(_MSC_VER)
#	define AIR_COMPILER_MSVC
#	define AIR_COMPLIER_NAME	vc
#	define AIR_SYMBOL_EXPORT	__declspec(dllexport)
#	define AIR_SYMBOL_IMPORT	__declspec(dllimport)

#	if _MSC_VER >= 1910
#		define AIR_COMPILER_VERSION 141
#	elif _MSC_VER >= 1900
#		define AIR_COMPILER_VERSION	140
#	elif _MSC_VER >= 1800
#		define AIR_COMPILER_VERSION 120
#	else
#		error "Unsupported compiler version. Please install vc12 or up."
#	endif

#if _MSVC_LANG > 201402
#define AIR_CXX17_LIBRARY_ANY_SUPPORT
#define AIR_CXX17_LIBRARY_OPTIONAL_SUPPORT
#define AIR_CXX17_LIBRARY_STRING_VIEW_SUPPORT
#endif
#define AIR_CXX17_LIBRARY_SIZE_AND_MORE_SUPPORT
#define AIR_TS_LIBRARY_FILESYSTEM_SUPPORT


#	if AIR_COMPILER_VERSION >= 140
#		define AIR_CPP11_CORE_CONSTEXPR_SUPPORT
#		define AIR_CXX11_CORE_NOEXCEPT_SUPPORT
#		define AIR_TS_LIBRARY_FILESYSTEM_V3_SUPPORT
#	else
#		define AIR_TS_LIBRARY_FILESYSTEM_V2_SUPPORT
#	endif
#	pragma warning(disable: 4251) // STL classes are not dllexport.
#	pragma warning(disable: 4275) // Derived from non dllexport classes.
#	pragma warning(disable: 4503) // Some decorated name in boost are very long.
#	pragma warning(disable: 4819) // Allow non-ANSI characters.
#	ifndef _CRT_SECURE_NO_DEPRECATE
#	define _CRT_SECURE_NO_DEPRECATE
#	endif

#	ifndef _SCL_SECURE_NO_DEPRECATE
#	define _SCL_SECURE_NO_DEPRECATE
#	endif

#	define AIR_ATTRIBUTE_NORETURN __declspec(noreturn)
#	define AIR_BUILTIN_UNREACHABLE __assume(false)

#else
#error "Unknown compiler. Please install vc, g++ or clang."
#	endif


#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#	define AIR_PLATFORM_WINDOWS
#	define AIR_HAS_DECLSPEC
#	if defined(_WIN64)
#		define AIR_PLATFORM_WIN64
#	else
#		define AIR_PLATFORM_WIN32
#	endif
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	ifndef WINDOWS_LEAD_AND_MEAN
#		define WINDOWS_LEAD_AND_MEAN
#	endif

#	ifndef BOOST_ALL_DYN_LINK
#		define BOOST_ALL_DYN_LINK
#	endif

#	if defined(__MINGW32__)
#		define AIR_COMPILER_NAME mgw
#		include<_mingw.h>
#	else
#		include <sdkddkver.h>
#	endif

#	ifndef _WIN32_WINNT_WIN7
#		define _WIN32_WINNT_WIN7 0x0601
#	endif
#	ifndef _WIN32_WINNT_WINBLUE
#		define _WIN32_WINNT_WINBLUE 0x0603
#	endif
#	ifndef _WIN32_WINNT_WIN10
#		define _WIN32_WINNT_WIN10 0x0A00
#	endif

#	if (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
#		include <winapifamily.h>
#	if defined(WINAPI_FAMILY)
#		if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
#			define AIR_PLATFORM_WINDOWS_DESKTOP
#		else
#			define ARI_PLATFORM_WINDOWS_STORE
#		endif
#	else
#		define AIR_PLATFORM_WINDOWS_DESKTOP
#	endif
#	else
#	define AIR_PLATFORM_WINDOWS_DESKTOP
#	endif
#elif defined(__ANDROID__)
#define AIR_PLATFORM_ANDROID
#elif defined(__CYGWIN__)
#define AIR_PLATFORM_CYGWIN
#elif defined(linux) || defined(__linux) || defined(__linux__)
#define AIR_PLATFORM_LINUX
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#define AIR_PLATFORM_IOS
#else
#define AIR_PLATFORM_DARWIN
#endif
#else
#error "Unknown platform. the supported target platform are Windows, Android, Linux, OSX, and IOS."

#endif


#if defined(AIR_COMPILER_MSVC)
	#if defined(_M_X64)
		#define AIR_CPU_X64
		#define AIR_COMPILER_TARGET x64
	#elif defined(_M_IX86)
		#define AIR_CPU_X86
		#define AIR_COMPILER_TARGET	X86
	#elif defined(_M_ARM)
		#define AIR_CPU_ARM
		#define AIR_COMPILER_TARGET	arm
	#else
		#error "Unknown CPU type. In vc, x86, x64 and arm are supported."
	#endif
#elif defined(AIR_COMPILER_GCC) || defined(AIR_COMPILER_CLANG)
#if defined(__x86_64__)
#define AIR_CPU_X64
#define AIR_COMPILER_TARGET x64
#elif defined(__i386__)
#define AIR_CPU_X86
#define AIR_COMPILER_TARGET x86
#elif defined(__arm__)
#define AIR_CPU_ARM
#define AIR_COMPILER_TARGET arm

#elif defined(__aarch64__)
#define AIR_CPU_ARM64
#define AIR_COMPILER_TARGET arm64
#else
#error "Unknown CPU type. In g++/clang, x86, x64, arm and arm64 are supported"
#endif
#endif


#if defined(AIR_CPU_ARM) || defined(AIR_CPU_ARM64)
#if defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__)
#define  AIR_BIG_ENDIAN
#else
#define  AIR_LITTLE_ENDIAN
#endif
#elif defined(AIR_CPU_X86) || defined(AIR_CPU_X64) || defined(AIR_PLATFORM_WINDOWS)
#define AIR_LITTLE_ENDIAN
#else
#error "Unknown CPU endian."

#endif


#ifdef AIR_CPU_X64
	#define AIR_SSE_SUPPORT
	#define AIR_SSE2_SUPPORT
	#define AIR_X64_SUPPORT
	#if defined(AIR_COMPILER_MSVC)
		#ifdef __AVX__
			#define AIR_AVX_SUPPORT
		#endif
		#ifdef __AVX2__
			#define AIR_AVX2_SUPPORT
		#endif
#elif defined(AIR_COMPILER_GCC) || defined(AIR_COMPILER_CLANG)
#ifdef __SSE3__
#define AIR_SSE3_SUPPORT
#endif
#ifdef __SSSE3__
#define AIR_SSSE3_SUPPORT
#endif
#ifdef __SSE4_1__
#define AIR_SSE4_1_SUPPORT
#endif
#ifdef __SSE4_2__
#define AIR_SSE4_2_SUPPORT
#endif
#ifdef __AVX__
#define AIR_AVX_SUPPORT
#endif
#ifdef __AVX2__
#define AIR_AVX2_SUPPORT
#endif

#endif
#elif defined AIR_CPU_X86



#if defined(AIR_COMPILER_GCC) || defined(AIR_COMPILER_CLANG)
#ifdef __MMX__
#define AIR_MMX_SUPPORT
#endif
#ifdef __SSE__
#define AIR_SSE_SUPPORT
#endif
#ifdef __SSE2__
#define AIR_SSE2_SUPPORT
#endif
#ifdef __SSE3__
#define AIR_SSE3_SUPPORT
#endif
#ifdef __SSSE3__
#define AIR_SSSE3_SUPPORT
#endif
#ifdef __SSE4_1__
#define AIR_SSE4_1_SUPPORT
#endif
#ifdef __SSE4_2__
#define AIR_SSE4_2_SUPPORT
#endif
#ifdef __AVX__
#define AIR_AVX_SUPPORT
#endif
#ifdef __AVX2__
#define AIR_AVX2_SUPPORT
#endif
#endif
#elif defined AIR_CPU_ARM
	#if defined(AIR_COMPILER_MSVC)
		#define AIR_NEON_SUPPORT
	#elif defined(AIR_COMPILER_GCC) || defined(AIR_COMPILER_CLANG)
		#ifdef __ARM_NEON__
			#define AIR_NEON_SUPPORT
		#endif
	#endif
#elif defined AIR_CPU_ARM64
#define AIR_NEON_SUPPORT
#endif

#if defined(AIR_COMPILER_MSVC) || defined(AIR_COMPILER_GCC) || defined(AIR_COMPILER_CLANG)
#define AIR_HAS_STRUCT_PACK
#endif

//#define FLAMEMATH

#ifndef BOOST_SYSTEM_NO_DEPRECATED
#define BOOST_SYSTEM_NO_DEPRECATED
#endif


#if defined(AIR_PLATFORM_WINDOWS) && ((defined(AIR_COMPILER_GCC) && defined(AIR_CPU_X64)) || defined(AIR_COMPILER_CLANG))
#ifndef BOOST_USE_WINDOWS_H
#define BOOST_USE_WINDOWS_H
#endif
#endif

#ifndef BOOST_DATE_TIME_NO_LIB
#define BOOST_DATE_TIME_NO_LIB
#endif

#if defined(AIR_PLATFORM_WINDOWS_DESKTOP) || defined(AIR_PLATFORM_LINUX) || defined(AIR_PLATFORM_DARWIN)
#define AIR_IS_DEV_PLATFORM 1
#else
#define AIR_IS_DEV_PLATFORM 0
#endif

#endif