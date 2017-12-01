#ifndef _WindowsPlatformProperties_H_
#define _WindowsPlatformProperties_H_
#pragma once
#include "GenericPlatform/GenericPlatformProperties.h"

template<bool HAS_EDITOR_DATA, bool IS_DEDICATED_SERVER, bool IS_CLIENT_ONLY>
struct WindowsPlatformProperties : public GenericPlatformProperties
{
	static FORCEINLINE bool requiresCookedData()
	{
		return false;
	}
};



#endif
