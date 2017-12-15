#ifndef _World_H_
#define _World_H_
#pragma once
#include "Context.h"
#include "EngineType.hpp"
namespace Air
{
	class AIR_CORE_API World
	{
	public:
		bool isGameWorld() const;

	private:
		EWorldType::Type mWorldType;
	};




}
#endif
