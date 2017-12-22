#ifndef _Level_H_
#define _Level_H_
#pragma once

namespace Air
{
	class GameObject;
	class World;
	class Level : public Object
	{
	public:
		std::vector<GameObject*> mActors;
		World* mOwningWorld;
		bool mIsVisible;
		class TickTaskLevel*	mTickTaskLevel;
	};
}



#endif
