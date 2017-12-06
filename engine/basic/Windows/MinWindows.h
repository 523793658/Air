#ifndef _MinWindows_H_
#define _MinWindows_H_
#pragma once
#include "HAL/CoreTypes.h"

#if defined(_WINDOWS_) && !defined(MINIMAL_WINDOWS_INCLUDE)
#pragma message ( " " )
#pragma message ( "You have included windows.h before MinWindows.h" )
#pragma message ( "All useless stuff from the windows headers won't be excluded !!!" )
#pragma message ( " " )
#endif
#include <stdint.h>
#define MINIMAL_WINDOWS_INCLUDE
#define NOMINMAX
#include <windows.h>
#endif
