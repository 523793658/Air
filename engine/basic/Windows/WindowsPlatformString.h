#ifndef _WindowsPlatformString_H_
#define _WindowsPlatformString_H_
#pragma once
#include "HAL/CoreTypes.h"
#include "Windows/WindowsHWrapper.h"
struct WindowsPlatformString
{
	static int WideCharToMultiByte(const wchar_t* source, uint32_t lengthWM1, char* dest, uint32_t lengthA)
	{
		return ::WideCharToMultiByte(CP_ACP, 0, source, lengthWM1 + 1, dest, lengthA, NULL, NULL);
	}

	static int MultiByteToWideChar(const char *Source, TCHAR *Dest, uint32_t LengthM1)
	{
		return ::MultiByteToWideChar(CP_ACP, 0, Source, LengthM1 + 1, Dest, LengthM1 + 1);
	}



};

typedef WindowsPlatformString PlatformString;

#endif
