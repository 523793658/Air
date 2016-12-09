#ifndef _Config_H_
#define _Config_H_

#if !defined(__cplusplus)
#error C++ compiler required.
#endif

#if defined(DEBUG) | defined(_DEBUG)
#define AIR_DEBUG
#endif

#define KFL_STRINGIZE(X) KFL_DO_STRINGIZE(X)
#define KFL_DO_STRINGIZE(X) #X

#if defined(_MSC_VER)
#	define AIR_COMPILER_MSVC
#	define AIR_COMPLIER_NAME	vc
#	define AIR_SYMBOL_EXPORT	__declspec(dllexport)
#	define AIR_SYMBOL_IMPORT	__declspec(dllimport)
#	if _MSC_VER >= 1900
#		define AIR_COMPILER_VERSION	140
#	elif _MSC_VER >= 1800
#		define AIR_COMPILER_VERSION 120
#	else
#		error "Unsupported compiler version. Please install vc12 or up."
#	endif

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
#		if !defined(__clang__)
#			define KLAYGE_COMPILER_NAME mgw
#			include <_mingw.h>
#		endif
#		ifndef WINVER
#			define WINVER 0x0501
#		endif
#	else
#		include <sdkddkver.h>
#	endif

#	ifndef _WIN32_WINNT_WINXP
#		define _WIN32_WINNT_WINXP 0x0501
#	endif
#	ifndef _WIN32_WINNT_VISTA
#		define _WIN32_WINNT_VISTA 0x0600
#	endif
#	ifndef _WIN32_WINNT_WIN7
#		define _WIN32_WINNT_WIN7 0x0601
#	endif
#	ifndef _WIN32_WINNT_WIN8
#		define _WIN32_WINNT_WIN8 0x0602
#	endif
#	ifndef _WIN32_WINNT_WINBLUE
#		define _WIN32_WINNT_WINBLUE 0x0603
#	endif
#	ifndef _WIN32_WINNT_WIN10
#		define _WIN32_WINNT_WIN10 0x0A00
#	endif

#	if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
#		include <winapifamily.h>
#	if defined(WINAPI_FAMILY)
#		if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
#			define AIR_PLATFORM_WINDOWS_DESKTOP
#		else
#			if WINAPI_FAMILY == WINAPI_FAMILY_PC_APP
#				if (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
#					define KLAYGE_PLATFORM_WINDOWS_UWP
#				endif
#				define KLAYGE_PLATFORM_WINDOWS_STORE
#			elif WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#				define KLAYGE_PLATFORM_WINDOWS_PHONE
#			endif
#			define KLAYGE_PLATFORM_WINDOWS_RUNTIME
#		endif
#	else
#		define KLAYGE_PLATFORM_WINDOWS_DESKTOP
#	endif
#	else
#	define KLAYGE_PLATFORM_WINDOWS_DESKTOP
#	endif
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
	#endif
#elif defined AIR_CPU_X86
	#if defined(KLAYGE_COMPILER_MSVC)
		#if 600 == _M_IX86
			#define KLAYGE_MMX_SUPPORT
		#endif

		#if 1 == _M_IX86_FP
			#define KLAYGE_SSE_SUPPORT
		#elif 2 == _M_IX86_FP
			#define KLAYGE_SSE_SUPPORT
			#define KLAYGE_SSE2_SUPPORT
			#ifdef __AVX__
				#define KLAYGE_AVX_SUPPORT
			#endif
			#ifdef __AVX2__
				#define KLAYGE_AVX2_SUPPORT
			#endif
		#endif
	#endif
#elif defined AIR_CPU_ARM
	#if defined(KLAYGE_COMPILER_MSVC)
		#define KLAYGE_NEON_SUPPORT
	#elif defined(KLAYGE_COMPILER_GCC) || defined(KLAYGE_COMPILER_CLANG)
		#ifdef __ARM_NEON__
			#define KLAYGE_NEON_SUPPORT
		#endif
	#endif
#elif 
defined AIR_CPU_ARM64
#endif

#define FLAMEMATH

#endif