#ifndef _ModuleManager_H_
#define _ModuleManager_H_
#pragma once
#include "basic/include/ITargetPlatformManagerModule.h"
#include "basic/include/STLUnorderMap.hpp"
#include <vector>
namespace Air
{
	class ModuleInfo
	{
	public:
		std::string mOriginalFileName;
		std::string mFileName;
		void* mHandle;
		std::shared_ptr<IModuleInterface> mModule;
		bool mWasUnloadedAtShutdown;
		int32_t mLoadOrder;
		static int32_t mCurrentLoadOrder;
	public:
		ModuleInfo()
			:mHandle(nullptr), mWasUnloadedAtShutdown(false), mLoadOrder(mCurrentLoadOrder++)
		{}
	};

	typedef std::shared_ptr<ModuleInfo> ModuleInfoPtr;


	class IModuleInterface;
	class ModuleManager
	{
	public:
		~ModuleManager();
		static ModuleManager& get();

	public:

		bool isModuleLoaded(const std::string& name);

		std::shared_ptr<IModuleInterface> loadModule(const std::string  name, bool wasReloaded = false);

		std::shared_ptr<IModuleInterface> getModule(const std::string_view name);


		template<typename TModuleInterface>
		static TModuleInterface* getModulePtr(const std::string name)
		{
			ModuleManager& moduleManager = ModuleManager::get();
			if (!moduleManager.isModuleLoaded(name))
			{
				return nullptr;
			}
			return static_cast<TModuleInterface*>(moduleManager.getModule(name).get());
		}

		template<typename TModuleInterface>
		static TModuleInterface* LoadModulePtr(const std::string moduleName)
		{
			ModuleManager& moduleManager = ModuleManager::get();
			if (!moduleManager.isModuleLoaded(moduleName))
			{
				moduleManager.loadModule(moduleName);
			}
			return getModulePtr<TModuleInterface>(moduleName);
		}

		void findModules(const wchar_t* wildcardWithoutExtension, std::vector<std::string>& outModules) const;

		template<typename TModuleInterface>
		static TModuleInterface& getModuleChecked(const std::string& moduleName)
		{
			ModuleManager& moduleManager = ModuleManager::get();
			BOOST_ASSERT(moduleManager.isModuleLoaded(moduleName));
			return (TModuleInterface&)(*moduleManager.getModule(moduleName));
		}

		template<typename TModuleInterface>
		static TModuleInterface& loadModuleChecked(const std::string moduleName)
		{
			ModuleManager& moduleManager = ModuleManager::get();
			if (!moduleManager.isModuleLoaded(moduleName))
			{
				moduleManager.loadModule(moduleName);
			}
			return getModuleChecked<TModuleInterface>(moduleName);
		}
		void addBinariesDirectory(const std::string inDirectory, bool isGameDirectory);
	private:
		std::vector<std::string> mGameBinariesDirectories;
		std::vector<std::string> mEngineBinariesDirectories;
		std::unordered_map<std::string, ModuleInfoPtr> mModules;
	private:
		ModuleInfoPtr findModule(std::string inModuleName);

	};

	

}
#endif
