#ifndef _WindowsSystemIncludes_H_
#define _WindowsSystemIncludes_H_
#pragma once

#define SAFE_RELEASE(p) {if(p){(p)->Release(); (p) = NULL;}}

#include <intrin.h>
#include <stdint.h>
#include <tchar.h>
#if defined(__clang__) || (defined(_MSC_VER) && (_MSC_VER >= 1900))
#include <intsafe.h>
#include <strsafe.h>
#endif

#endif
