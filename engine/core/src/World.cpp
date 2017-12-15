#include "World.hpp"
namespace Air
{
	bool World::isGameWorld() const
	{
		return mWorldType == EWorldType::Game || mWorldType == EWorldType::PIE || mWorldType == EWorldType::GamePreview;
	}
}