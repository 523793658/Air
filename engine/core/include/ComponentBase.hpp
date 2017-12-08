#ifndef _ComponentBase_H_
#define _ComponentBase_H_
#pragma once
#include "PreDeclare.h"
#include "EngineType.hpp"
#include "Type.hpp"


namespace Air
{
	class World;

	enum class UpdateTransformFlags : int32_t
	{
		None = 0x0,
		SkipPhysicsUpdate = 0x1,
		PropagateFromParent = 0x2
	};


	class AIR_CORE_API ComponentBase
	{
	public:
		ComponentBase();

		virtual void onRegister();

		virtual void onUnRegister();

		virtual void updateComponentToWorld(UpdateTransformFlags updateTransformFlags = UpdateTransformFlags::None, ETeleportType teleport = ETeleportType::None) {}

		class GameObject* getOwner() const;

		virtual void activate(bool bReset = false);

	protected:
		uint32_t mCanEverAffectNavigation{ 1 };

	private:
		bool mRegistered{ false };
		bool mAutoActivate{ false };
		World* mWorldPrivate;
		mutable GameObject* mOwner;

		bool mNavigationRelevant{ false };
	};





}




#endif
