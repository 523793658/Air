#include "Context.h"
#include "Engine.h"

#include "SingletonManager.hpp"
namespace Air
{

	RenderFactory& SingletonManager::getRenderFactoryInstance()
	{
		return Engine::getInstance().getRenderFactoryInstance();
	}

	SceneManager& SingletonManager::getSceneManagerInstance()
	{
		return Engine::getInstance().getSceneManangerInstance();
	}

	InputFactory& SingletonManager::getInputFactoryInstance()
	{
		return Engine::getInstance().getInputFactoryInstance();
	}

}

