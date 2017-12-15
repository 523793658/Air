#ifndef _TickTaskManagerInterface_H_
#define _TickTaskManagerInterface_H_
#pragma once
#include "basic/include/Basic.h"
#include "basic/include/EngineBaseType.hpp"
#include <vector>
namespace Air
{

class TickTaskLevel;
class Level;
class World;
class TickTaskManagerInterface
{
public:
	virtual ~TickTaskManagerInterface() {}
	virtual TickTaskLevel* allocateTickTaskLevel() = 0;
	virtual void freeTickTaskLevel(TickTaskLevel* tickTaskLevel) = 0;
	virtual void startFrame(World* inWorld, float deltaSeconds, ELevelTick tickType, const std::vector<Level*>& LevelsToTick) = 0;
	virtual void runPauseFrame(World* inWorld, float deltaSecond, ELevelTick tickType, const std::vector<Level*>& LevelsToTick) = 0;

	virtual void runTickGroup(ETickingGroup group, bool bBlockTillComplete) = 0;
	virtual void endFrame() = 0;

	//virtual void dumpAllTickFunctions()
	static TickTaskManagerInterface& get();
};
}

#endif