#include "Basic.h"
#include "ResIdentifier.h"

#ifdef AIR_PLATFORM_WINDOWS
	#include <windows.h>
#ifdef AIR_PLATFORM_WINDOWS_RUMTIME
	#include "Thread.h"
#endif

#else
#include "Thread.h"
#include <cerrno>
#include <cstdlib>
#include <cwchar>
#include <clocale>
#endif

#include <vector>
#include <algorithm>
#include <boost/boost/assert.hpp>

#include "Util.h"