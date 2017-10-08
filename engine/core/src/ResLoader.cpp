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

namespace
{
	std::mutex singleton_mutex;
}

namespace Air
{
	std::unique_ptr<ResLoader> ResLoader::mInstance;

	ResLoader::ResLoader()
		:mQuit(false)
	{
#if defined AIR_PLATFORM_WINDOWS
#if defined AIR_PLATFORM_WINDOWS_DESKTOP
		char buf[MAX_PATH];
		::GetModuleFileNameA(nullptr, buf, sizeof(buf));
		mExePath = buf;
		mExePath = mExePath.substr(0, mExePath.rfind("\\"));
#else
#endif
#endif
		mPaths.push_back("");
#if defined AIR_PLATFORM_WINDOWS_RUNTIME
		this->addPath("Assets/");
#else
		this->addPath("");
#if defined AIR_PLATFORM_WINDOWS_DESKTOP
		::GetCurrentDirectoryA(sizeof(buf), buf);
		char* colon = std::find(buf, buf + sizeof(buf), ':');
		BOOST_ASSERT(colon != buf + sizeof(buf));
		colon[1] = '/';
		colon[2] = '\0';
		this->addPath(buf);
#endif
#if defined(AIR_PLATFORM_WINDOWS_DESKTOP)
		this->addPath("../");
		this->addPath("../../../");
#endif
#endif
		mLoadingThread = MakeUniquePtr<joiner<void >> (Engine::getInstance().getThreadPool()(
			std::bind(&ResLoader::loadingThreadFunc, this)));
	}

	void ResLoader::destroy()
	{
		mInstance.reset();
	}

	void ResLoader::loadingThreadFunc()
	{
		while (!mQuit)
		{
			std::pair<ResLoadingDescPtr, std::shared_ptr<volatile LoadingStatus>> res_pair;
			while (mLoadingResQueue.pop(res_pair))
			{
				if (LS_Loading == *res_pair.second)
				{
					res_pair.first->subThreadStage();
					*res_pair.second = LS_Complete;
				}
			}
			Sleep(10);
		}
	}

	void ResLoader::suspend()
	{

	}
	void ResLoader::resume()
	{

	}

	std::string ResLoader::getAbsPath(std::string const & path)
	{
		using namespace std::experimental;
		filesystem::path new_path(path);
#ifdef AIR_TS_LIBRARY_FILESYSTEM_V2_SUPPORT
		if(!new_path.is_complete())
#else
		if (!new_path.is_absolute())
#endif
		{
			filesystem::path full_path = filesystem::path(mExePath) / new_path;
			if (!filesystem::exists(full_path))
			{
#ifndef AIR_PLATFORM_ANDROID
#ifdef AIR_TS_LIBRARY_FILESYSTEM_V2_SUPPORT
				full_path = filesystem::current_path<filesystem::path>() / new_path;
#else
				try
				{
					full_path = filesystem::current_path() / new_path;
				}
				catch (...)
				{
					full_path = new_path;
				}
#endif
				if (!filesystem::exists(full_path))
				{
					return "";
				}
#else
				return ""
#endif
			}
			new_path = full_path;
		}
		std::string ret = new_path.string();
#if defined AIR_PLATFORM_WINDOWS
		std::replace(ret.begin(), ret.end(), '\\', '/');
#endif
		return ret;
	}


	std::string ResLoader::getRealPath(std::string const & path)
	{
		std::string abs_path = this->getAbsPath(path);
		if (!abs_path.empty() && (abs_path[abs_path.length() - 1] != '/'))
		{
			abs_path.push_back('/');
		}
		return abs_path;
	}

	void ResLoader::addPath(std::string const & path)
	{
		std::lock_guard<std::mutex> lock(pathsMutex);
		std::string real_path = this->getRealPath(path);
		if (!real_path.empty())
		{
			mPaths.push_back(real_path);
		}
	}
	void ResLoader::delPath(std::string const & path)
	{
		std::lock_guard<std::mutex> lock(pathsMutex);
		std::string real_path = this->getRealPath(path);
		if (!real_path.empty())
		{
			auto iter = std::find(mPaths.begin(), mPaths.end(), real_path);
			if (iter != mPaths.end())
			{
				mPaths.erase(iter);
			}
		}
	}

	ResIdentifierPtr ResLoader::open(std::string const & name)
	{
		using namespace std::experimental;
		{
			std::lock_guard<std::mutex> lock(pathsMutex);
			for (auto const & path : mPaths)
			{
				std::string res_name(path + name);
#if defined AIR_PLATFORM_WINDOWS
				std::replace(res_name.begin(), res_name.end(), '\\', '/');
#endif
				filesystem::path res_path(res_name);
				if (filesystem::exists(res_path))
				{
#ifdef AIR_TS_LIBRARY_FILESYSTEM_V3_SUPPORT
					uint64_t timestamp = filesystem::last_write_time(res_path).time_since_epoch().count();
#else
					uint64_t timestamp = filesystem::last_write_time(res_path);
#endif
					return MakeSharedPtr<ResIdentifier>(name, timestamp, MakeSharedPtr<std::ifstream>(res_name.c_str(), std::ios_base::binary));
				}
				else
				{
					//不存在则看压缩包中释放存在
					/*std::string password;
					std::string internal_name;
					ResIdentifierPtr pkt_file = LocatePkt(name, res_name, password, internal_name);
					if (pkt_file && *pkt_file)
					{
						std::shared_ptr<std::iostream> packet_file = MakeSharedPtr<std::stringstream>();
						
					}*/
				}
			}
		}
		return ResIdentifierPtr();
	}


	ResLoader::~ResLoader()
	{
		mQuit = true;
		(*mLoadingThread)();
	}



	


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
		return res;
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

	ResLoader& ResLoader::getInstance()
	{
		if (!mInstance)
		{
			std::lock_guard<std::mutex> lock(singleton_mutex);
			if (!mInstance)
			{
				mInstance = MakeUniquePtr<ResLoader>();
			}
		}
		return *mInstance;
	}

	std::shared_ptr<void> ResLoader::syncQuery(ResLoadingDescPtr const & res_desc)
	{
		this->removeUnrefResources();
		std::shared_ptr<void> loaded_res = this->findMatchLoadedResource(res_desc);
		std::shared_ptr<void> res;
		if (loaded_res)
		{
			if (res_desc->getStateLess())
			{
				res = loaded_res;
			}
			else
			{
				res = res_desc->cloneResourceFrom(loaded_res);
				if (res != loaded_res)
				{
					this->addLoadedResource(res_desc, res);
				}
			}
		}
		else
		{
			std::shared_ptr<volatile LoadingStatus> async_is_done;
			bool found = false;
			{
				std::lock_guard<std::mutex> lock(mLoadingMutex);
				for (auto const & lrq : mLoadingRes)
				{
					if (lrq.first->match(*res_desc))
					{
						res_desc->copyDataFrom(*lrq.first);
						res = lrq.first->getResource();
						async_is_done = lrq.second;
						found = true;
						break;
					}
				}
			}
			if (found)
			{
				if (!res_desc->getStateLess())
				{
					std::lock_guard<std::mutex> lock(mLoadingMutex);
					mLoadingRes.emplace_back(res_desc, async_is_done);
				}
			}
			else
			{
				if (res_desc->hasSubThreadStage())
				{
					res = res_desc->createResource();
					async_is_done = MakeSharedPtr<LoadingStatus>(LS_Loading);
					{
						std::lock_guard<std::mutex> lock(mLoadingMutex);
						mLoadingRes.emplace_back(res_desc, async_is_done);
					}
					mLoadingResQueue.push(std::make_pair(res_desc, async_is_done));
				}
				else
				{
					res_desc->mainThreadStage();
					res = res_desc->getResource();
					this->addLoadedResource(res_desc, res);
				}
			}
		}
		return res;
	}

	std::shared_ptr<void> ResLoader::findMatchLoadedResource(ResLoadingDescPtr const & res_desc)
	{
		std::lock_guard<std::mutex> lock(mLoadedMutex);
		std::shared_ptr<void> loadedRes;
		for (auto const & lr : mLoadedRes)
		{
			if (lr.first->match(*res_desc))
			{
				loadedRes = lr.second.lock();
				break;
			}
		}
		return loadedRes;
	}

	void ResLoader::addLoadedResource(ResLoadingDescPtr const & res_desc, std::shared_ptr<void> const & res)
	{
		std::lock_guard<std::mutex> lock(mLoadedMutex);
		bool found = false;
		for (auto& c_desc : mLoadedRes)
		{
			if (c_desc.first == res_desc)
			{
				c_desc.second = std::weak_ptr<void>(res);
				found = true;
				break;
			}
		}
		if (!found)
		{
			mLoadedRes.emplace_back(res_desc, std::weak_ptr<void>(res));
		}
	}

	void ResLoader::removeUnrefResources()
	{
		std::lock_guard<std::mutex> lock(mLoadedMutex);
		for (auto iter = mLoadedRes.begin(); iter != mLoadedRes.end(); )
		{
			if (iter->second.lock())
			{
				++iter;
			}
			else
			{
				iter = mLoadedRes.erase(iter);
			}
		}
	}

	void ResLoader::update()
	{
		std::vector<std::pair<ResLoadingDescPtr, std::shared_ptr<volatile LoadingStatus>>> tmpLoadingRes;
		{
			std::lock_guard<std::mutex> lock(mLoadingMutex);
			tmpLoadingRes = mLoadingRes;
		}
		for (auto & lrq : tmpLoadingRes)
		{
			if (LS_Complete == *lrq.second)
			{
				ResLoadingDescPtr const & res_desc = lrq.first;
				std::shared_ptr<void> res;
				std::shared_ptr<void> loaded_res = this->findMatchLoadedResource(res_desc);
				if (loaded_res)
				{
					if (!res_desc->getStateLess())
					{
						res = res_desc->cloneResourceFrom(loaded_res);
						if (res != loaded_res)
						{
							this->addLoadedResource(res_desc, res);
						}
					}
				}
				else
				{
					res_desc->mainThreadStage();
					res = res_desc->getResource();
					this->addLoadedResource(res_desc, res);
				}
				*lrq.second = LS_CanBeRemoved;
			}
		}
		{
			std::lock_guard<std::mutex> lock(mLoadingMutex);
			for (auto iter = mLoadingRes.begin(); iter != mLoadingRes.end();)
			{
				if (LS_CanBeRemoved == *(iter->second))
				{
					iter = mLoadingRes.erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}
	}
}