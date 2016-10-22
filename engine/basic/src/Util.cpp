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
#include <boost/assert.hpp>

#include "Util.h"

namespace Air 
{
	std::string& convert(std::string& strDest, std::string const & strSrc)
	{
		strDest = strSrc;
		return strDest;
	}
	std::string& convert(std::string& strDest, std::wstring const & wstrSrc)
	{
#if defined AIR_PLATFORM_WINDOWS
		int const mbs_len = WideCharToMultiByte(CP_ACP, 0, wstrSrc.c_str(), static_cast<int>(wstrSrc.size()), nullptr, 0, nullptr, nullptr);
		std::vector<char> tmp(mbs_len + 1);
		WideCharToMultiByte(CP_ACP, 0, wstrSrc.c_str(), static_cast<int>(wstrSrc.size()), &tmp[0], mbs_len, nullptr, nullptr);
#endif
		strDest.assign(tmp.begin(), tmp.end() - 1);
		return strDest;
	}
	std::wstring& convert(std::wstring & wstrDest, std::string const & strSrc)
	{
#if defined AIR_PLATFORM_WINDOWS
		int const wcs_len = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), static_cast<int>(strSrc.size()), nullptr, 0);
		std::vector<wchar_t> tmp(wcs_len + 1);
		MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), static_cast<int>(strSrc.size()), &tmp[0], wcs_len);
#endif
		wstrDest.assign(tmp.begin(), tmp.end() - 1);
		return wstrDest;
	}
	std::wstring& convert(std::wstring& wstrDst, std::wstring const & wstrSrc)
	{
		wstrDst = wstrSrc;
		return wstrDst;
	}
}

