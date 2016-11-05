#include "basic/include/Basic.h"
#include "basic/include/ResIdentifier.h"
#ifdef AIR_PLATFORM_WINDOWS
#include <windows.h>
#else
#endif
#include "basic/include/DllLoader.h"

namespace Air
{
	DllLoader::DllLoader()
		:mDllHandle(nullptr)
	{

	}

	DllLoader::~DllLoader()
	{
		this->free();
	}

	bool DllLoader::load(std::string const & dll_name)
	{
#ifdef AIR_PLATFORM_WINDOWS
#ifdef AIR_PLATFORM_WINDOWS_DESKTOP
		mDllHandle = static_cast<void*>(::LoadLibraryExA(dll_name.c_str(), nullptr, 0));
#else

#endif

#else
#endif
		return (mDllHandle != nullptr);
	}

	void DllLoader::free()
	{
		if (mDllHandle)
		{
#ifdef AIR_PLATFORM_WINDOWS
			::FreeLibrary(static_cast<HMODULE>(mDllHandle));
#endif // AIR_PLATFORM_WINDOWS

		}
	}

	void * DllLoader::getProcAddress(std::string const & proc_name)
	{
#ifdef AIR_PLATFORM_WINDOWS
		return reinterpret_cast<void*>(::GetProcAddress(static_cast<HMODULE>(mDllHandle), proc_name.c_str()));
#endif // AIR_PLATFORM_WINDOWS

	}

}


