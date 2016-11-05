#include "Engine.h"
#include "basic/include/Util.h"
#include <fstream>
#include <sstream>
#if defined(AIR_TS_LIBRARY_FILESYSTEM_V3_SUPPORT)
#include <experimental/filesystem>
#elif defined(AIR_TS_LIBRARY_FILESYSTEM_V2_SUPPORT)
#include <filesystem>
namespace std
{
	namespace experimental
	{
		namespace filesystem = std::tr2::sys;
	}
}
#else
#endif

#if defined AIR_PLATFORM_WINDOWS_DESKTOP
#include <windows.h>

#endif
#include "ResLoader.h"

namespace Air
{
	ResIdentifierPtr ResLoader::LocatePkt(std::string const & name, std::string const & res_name, std::string& password, std::string& internal_name)
	{
		using namespace std::experimental;
		ResIdentifierPtr res;
		std::string::size_type const pkt_offset(res_name.find("//"));
		if (pkt_offset != std::string::npos)
		{
			std::string pkt_name = res_name.substr(0, pkt_offset);
			filesystem::path pkt_path(pkt_name);
			if (filesystem::exists(pkt_path) && (filesystem::is_regular_file(pkt_path)
				|| filesystem::is_symlink(pkt_path)))
			{
				std::string::size_type const password_offset = pkt_name.find("|");
				if (password_offset != std::string::npos)
				{
					password = pkt_name.substr(password_offset + 1);
					pkt_name = pkt_name.substr(0, password_offset - 1);
				}
				internal_name = res_name.substr(pkt_offset + 2);
#ifdef AIR_TS_LIBRARY_FILESYSTEM_V3_SUPPORT
				uint64_t timestamp = filesystem::last_write_time(pkt_path).time_since_epoch().count();
#else
				uint64_t timestamp = filesystem::last_write_time(pkt_path);
#endif
				res = MakeSharedPtr<ResIdentifier>(name, timestamp, MakeSharedPtr<std::ifstream>(pkt_name.c_str(), std::ios_base::binary));
			}
		}
	}


	std::string ResLoader::locate(std::string const & name)
	{
		using namespace std::experimental;
		{
			std::lock_guard<std::mutex> lock(pathsMutex);
			for (auto const & path : mPaths)
			{
				std::string resName(path + name);
#if defined AIR_PLATFORM_WINDOWS
				std::replace(resName.begin(), resName.end(), '\\', '/');
#endif
				if (filesystem::exists(filesystem::path(resName)))
				{
					return resName;
				}
				else
				{
					//打包处理
					//std::string password;
					//std::string internal_name;
					//ResIdentifierPtr pkt_file = LocatePkt(name, resName, password, internal_name);
					//if (pkt_file && *pkt_file)
					//{
					//	if ()
					//}

				}
			}
		}
		return "";
	}
}