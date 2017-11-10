#ifndef _SingletonManager_H_
#define _SingletonManager_H_
#pragma once
#include "PreDeclare.h"

namespace Air
{
	class AIR_CORE_API SingletonManager
	{
	public:
		friend class Engine;
		
	public:
		static RenderFactory& getRenderFactoryInstance();

		static SceneManager& getSceneManagerInstance();

		static InputFactory& getInputFactoryInstance();
	};
}


#endif
