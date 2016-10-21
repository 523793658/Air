#ifndef _DllLoader_H_
#define _DllLoader_H_
#pragma once

#include <string>
#ifdef AIR_COMPILER_MSVC
#	define DLL_PREFIX ""
#else
#	define DLL_PREFIX "lib"
#endif

#if defined(AIR_PLATFORM_WINDOWS)
#	define DLL_EXT_NAME	"dll"
#endif

#define DLL_SUFFIX	AIR_OUTPUT_SUFFIX "." DLL_EXT_NAME

namespace Air
{
	class DllLoader
	{
	public:
		DllLoader();
		~DllLoader();
		bool load(std::string const & dll_name);
		void free();
		void* GetProcAddress(std::string const & proc_name);
	private:
		void* dll_handle_;
	};
}


#endif