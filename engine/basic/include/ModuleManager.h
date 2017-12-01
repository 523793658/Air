#ifndef _ModuleManager_H_
#define _ModuleManager_H_
#pragma once
#include "basic/include/ITargetPlatformManagerModule.h"
#include <vector>
namespace Air
{
	class IModuleInterface;
	class ModuleManager
	{
	public:
		~ModuleManager();
		static ModuleManager& get();

	public:

		bool isModuleLoaded(const std::string_view name);

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
		static TModuleInterface& getModuleChecked(const std::string_view moduleName)
		{
			ModuleManager& moduleManager = ModuleManager::get();
			BOOST_ASSERT(moduleManager.isModuleLoaded(moduleName));
			return (TModuleInterface)(*moduleManager.getModule(moduleName));
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

	};

	

}
#endif
