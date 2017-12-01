#ifndef _ITargetPlatformManagerModule_H_
#define _ITargetPlatformManagerModule_H_
#pragma once
#include "basic/include/ModuleInterface.hpp"
namespace Air
{
	class ITargetPlatformManagerModule : IModuleInterface
	{
	public:
		virtual const class IShaderFormat* findShaderFormat(std::string_view name) = 0;
	};
}
#endif
