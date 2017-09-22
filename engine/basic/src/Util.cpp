#include "basic/include/Basic.h"
#include "basic/include/ResIdentifier.h"

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

#include "basic/include/Util.h"

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

	template<>
	void EndianSwitch<2>(void* p)
	{
		uint8_t* bytes = static_cast<uint8_t*>(p);
		std::swap(bytes[0], bytes[1]);
	}

	template<>
	void EndianSwitch<4>(void* p)
	{
		uint8_t* bytes = static_cast<uint8_t*>(p);
		std::swap(bytes[0], bytes[3]);
		std::swap(bytes[1], bytes[2]);
	}

	template<>
	void EndianSwitch<8>(void* p)
	{
		uint8_t* bytes = static_cast<uint8_t*>(p);
		std::swap(bytes[0], bytes[7]);
		std::swap(bytes[1], bytes[6]);
		std::swap(bytes[2], bytes[5]);
		std::swap(bytes[3], bytes[4]);

	}

	std::string readShortString(ResIdentifierPtr const & res)
	{
		uint8_t len;
		res->read(&len, sizeof(len));
		std::string tmp;
		if (len > 0)
		{
			tmp.resize(len);
			res->read(&tmp[0], len * sizeof(tmp[0]));
		}
		return tmp;
	}

	std::wstring readShortWString(ResIdentifierPtr const & res)
	{
		uint8_t len;
		res->read(&len, sizeof(len));
		std::wstring tmp;
		if (len > 0)
		{
			tmp.resize(len);
			res->read(&tmp[0], len * sizeof(tmp[0]));
		}
		return tmp;
	}

	void writeShortString(std::ostream& os, std::string_view str)
	{
		uint8_t len = static_cast<uint8_t>(std::min(str.size(), static_cast<size_t>(255)));
		os.write(reinterpret_cast<char*>(&len), sizeof(len));
		if (len > 0)
		{
			os.write(str.data(), len * sizeof(str[0]));
		}
	}
}

