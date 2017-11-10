#ifndef _GameObject_H_
#define _GameObject_H_
#pragma once
#include "PreDeclare.h"
#include "Type.hpp"
namespace Air
{
	class AIR_CORE_API GameObject
	{
	public:
		GameObject();
		GameObject(std::string name);
		void addComponent(std::string className);
		void addComponent(AType type);
	private:
		std::vector<ComponentBasePtr> mComponents;
	};





}





#endif
