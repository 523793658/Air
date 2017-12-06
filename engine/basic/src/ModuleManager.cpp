#include "basic/include/Basic.h"
#include "basic/include/ModuleManager.h"
#include "boost/assert.hpp"
#include "HAL/FileSystem.h"
#include "HAL/Platform.h"
#include "basic/include/Globals.hpp"

#include <mutex>

namespace Air
{
	static std::mutex singletonMutex;
	void ModuleManager::addBinariesDirectory(const std::string inDirectory, bool isGameDirectory)
	{
		if (isGameDirectory)
		{
			mGameBinariesDirectories.push_back(inDirectory);
		}
		else
		{
			mEngineBinariesDirectories.push_back(inDirectory);
		}
		PlatformProcess::addDllDirectory(inDirectory);

		const std::string restrictedFolderNames[] =
		{ "NoRedist",
			"NotForLicensees",
			"CarefullyRedist"
		};
	}
	ModuleManager& ModuleManager::get()
	{
		static ModuleManager* mModuleManager = nullptr;
		if (mModuleManager == nullptr)
		{
			std::lock_guard<std::mutex> lock(singletonMutex);
			if (mModuleManager == nullptr)
			{
				BOOST_ASSERT(isInGameThread());
				mModuleManager = new ModuleManager();
#if PLATFORM_DESKTOP
				std::string restrictedFolderNames[] =
				{
					"NoRedist",
					"NotForLicensees",
					"CarefullyRedist"
				};
				std::string moduleDir = PlatformProcess::getModulesDirectory();
				for (auto folderName : restrictedFolderNames)
				{
					std::string restrictedFolder = moduleDir + "/" + folderName;
					if (filesystem::exists(restrictedFolder))
					{
						mModuleManager->addBinariesDirectory(restrictedFolder, false);
					}

				}
#endif
			}
		}
		return *mModuleManager;
	}
	ModuleInfoPtr ModuleManager::findModule(std::string inModuleName)
	{
		
		std::lock_guard<std::mutex> lock(singletonMutex);
		auto r = mModules.find(inModuleName);
		if (r != mModules.end())
		{
			return r->second;
		}
	}

	bool ModuleManager::isModuleLoaded(const std::string& name)
	{
		std::shared_ptr<ModuleInfo> moduleInfoPtr = findModule(name);
		if (moduleInfoPtr)
		{
			const ModuleInfo& moduleInfo = *moduleInfoPtr;
			if (moduleInfo.mModule)
			{
				return true;
			}
		}
		return false;
	}

}