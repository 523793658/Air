#ifndef _ComponentBase_H_
#define _ComponentBase_H_
#pragma once
#include "PreDeclare.h"
#include "EngineType.hpp"
#include "Type.hpp"
#include "basic/include/Basic.h"
#include "basic/include/EngineBaseType.hpp"


namespace Air
{
	class World;
	enum class UpdateTransformFlags : int32_t
	{
		None = 0x0,
		SkipPhysicsUpdate = 0x1,
		PropagateFromParent = 0x2
	};


	class AIR_CORE_API ComponentBase : public Object
	{
	public:
		ComponentBase();

		virtual void onRegister();

		virtual void onUnRegister();

		virtual void updateComponentToWorld(UpdateTransformFlags updateTransformFlags = UpdateTransformFlags::None, ETeleportType teleport = ETeleportType::None) {}

		class GameObject* getOwner() const;

		virtual void activate(bool bReset = false);

		FORCEINLINE bool isRegistered() const { return mRegistered; }

		bool modify(bool bAlwaysMarkDirty = true);

		void registerComponentWithWorld(World* inWorld);

		std::string getFullName();

		void onComponentCreated();

		void executeRegisterEvents();

		void registerAllComponentTickFunctions(bool bRegister);

		void registerComponentTickFunctions(bool bRegister);

		void initializeComponent();

		void beginPlay();

		bool setupComponentTickFunction(struct TickFunction* tickFunction);

	public:
		bool mAutoRegister{ false };

		bool mWantsInitializeComponent{ true };

		bool mHasBegunPlay{ true };
	protected:
		uint32_t mCanEverAffectNavigation{ 1 };

	private:
		bool mRegistered{ false };
		bool mAutoActivate{ false };
		World* mWorldPrivate;
		mutable GameObject* mOwner;

		bool mNavigationRelevant{ false };

		bool mTickFunctionsRegistered{ true };

		uint32_t mHasBeenCreated{ 1 };

		uint32_t mHasBeenInitialized{ 1 };

		struct ComponentTickFunction mPrimaryComponentTick;
	};
}




#endif
