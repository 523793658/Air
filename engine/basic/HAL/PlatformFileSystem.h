#ifndef _PlatformFileSystem_H_
#define _PlatformFileSystem_H_
#pragma once
#include "HAL/CoreTypes.h"


class PlatformFileSystem
{
	class IPlatformFile* mTopmostPlatformFile;
public:
	PlatformFileSystem();

	IPlatformFile& getPlatformFile();

	void setPlatformFile(IPlatformFile& newTopmostPlatformFile);
public:
	static PlatformFileSystem& get();
};


#endif
