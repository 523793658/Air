#ifndef _Platform_H_
#define _Platform_H_

#pragma once

#include "basic/include/Config.h"

#if !defined(AIR_PLATFORM_WINDOWS)
#define AIR_PLATFORM_WINDOWS 0
#endif

#include "GenericPlatform/GenericPlatform.h"

#if AIR_PLATFORM_WINDOWS
#include "Windows/WindowsPlatform.h"
#else
#error "Unknown Compiler"
#endif

#ifndef PLATFORM_DESKTOP
#error "PLATFORM_DESKTOP must be defined"
#endif


#ifndef PLATFORM_HAS_CPUID
#if defined(_M_IX86) || defined(__i386__) || defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
#define PLATFORM_HAS_CPUID			1
#else
#define PLATFORM_HAS_CPUID			0
#endif
#endif

#ifndef PLATFORM_COMPILER_CLANG
#if defined(__clang__)
#define PLATFORM_COMPILER_CLANG			1
#else
#define PLATFORM_COMPILER_CLANG			0
#endif
#endif

#ifndef PLATFORM_SEH_EXCEPTIONS_DISABLED
#define PLATFORM_SEH_EXCEPTIONS_DISABLED		0
#endif


#if !defined(TEXT)
#define TEXT_PASTE(x) L ## x
#define TEXT(x) TEXT_PASTE(x)
#endif


#endif