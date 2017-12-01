#ifndef _WindowsSystemInclude_H_
#define _WindowsSystemInclude_H_
#pragma once
#include "basic/include/Basic.h"
#include "basic/include/MinimalWindowsApi.h"


#define  SAFE_RELEASE(p) {if(p){(p)->Release(); (p) = NULL;}}

#include <intrin.h>
#include <stdint.h>
#include <tchar.h>

#if defined(__clang__) || (defined(_MSC_VER) && (_MSC_VER >= 1900))
#include <intsafe.h>
#include <strsafe.h>
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

#endif
