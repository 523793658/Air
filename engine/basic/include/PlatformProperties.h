#pragma once

#include "HAL/CoreTypes.h"
#include "GenericPlatform/GenericPlatformProperties.h"

#if AIR_PLATFORM_WINDOWS
#include "Windows/WindowsPlatformProperties.h"
typedef WindowsPlatformProperties<false, false, true> PlatformProperties;
#endif