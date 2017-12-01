#include "basic/include/Basic.h"
#include "boost/assert.hpp"
#include "Misc/CoreMisc.hpp"
#include "HAL/PlatformInclude.h"
#include "basic/include/Globals.hpp"
#include "basic/include/ModuleManager.h"
#include "basic/include/ErrorHanding.hpp"

namespace Air
{

class ITargetPlatformManagerModule* getTargetPlatformManager()
{
	static class ITargetPlatformManagerModule* SingletonInterface = nullptr;
	if (!PlatformProperties::requiresCookedData())
	{
		static bool bInitialized = false;
		if (!bInitialized)
		{
			BOOST_ASSERT(isInGameThread());
			bInitialized = true;
			SingletonInterface = ModuleManager::LoadModulePtr<ITargetPlatformManagerModule>("TargetPlatform");
		}
	}
	return SingletonInterface;
}

class ITargetPlatformManagerModule& getTargetPlatformManagerRef()
{
	class ITargetPlatformManagerModule* singletomInterface = getTargetPlatformManager();
	if (!singletomInterface)
	{
		AIR_UNREACHABLE("load module failed.");
	}
	return *singletomInterface;

}



}
