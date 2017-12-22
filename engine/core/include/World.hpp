#ifndef _World_H_
#define _World_H_
#pragma once
#include "Context.h"
#include "EngineType.hpp"
namespace Air
{
	enum class LevelCollectionType
	{
		DynamicSourceLevels,
		DynamicDuplicatedLevels,
		StaticLevels
	};

	struct AIR_CORE_API LevelCollection
	{
		LevelCollection();
	public:
		LevelCollectionType getType() const
		{
			return mCollectionType;
		}
	private:
		LevelCollectionType mCollectionType;
	};


	class AIR_CORE_API World
	{
	public:
		bool isGameWorld() const;
		const LevelCollection* getActiveLevelCollection() const
		{
			return  mActiveLevelCollection;
		}

		void setActiveLevelCollection(const LevelCollection* inCollection);
	private:
		EWorldType::Type mWorldType;
		LevelCollection* mActiveLevelCollection{ nullptr };

	public:
		class TickTaskLevel*	mTickTaskLevel;
	};




}
#endif
