#ifndef _EngineBaseType_H_
#define _EngineBaseType_H_
#pragma once
#include "basic/include/Basic.h"
namespace Air
{
	class GameObject;

	enum ELevelTick
	{
		ELT_TimeOnly = 0,
		ELT_ViewportOnly = 1,
		ELT_All = 2,
		ELT_PauseTick = 3
	};

	enum ETickingGroup
	{
		TG_PrePhysics,
		TG_StartPhysics,
		TG_DuringPhysics,
		TG_EndPhysics,
		TG_PostPhysics,
		TG_PostUpdateWork,
		TG_LastDemotable,
		TG_NewlySpawned,
		TG_MAX
	};

	struct TickFunction
	{
		bool isTickFunctionRegistered() const { return mRegistered; }

		void unRegisterTickFunction();

		bool isTickFunctionEnabled() const { return mTickState == ETickState::Enabled; }

		void setTickFunctionEnable(bool bInEnable);
	public:
		enum class ETickState : uint8_t
		{
			Disabled,
			Enabled,
			CoolingDown
		};

		ETickState mTickState;

		float mTickInterval;

		TickFunction* mNext;

		float mRelativeTickCoolDown;

		bool mStartWithTickEnabled{ true };

	private:
		

		bool mRegistered{ true };


		class TickTaskLevel*				mTickTaskLevel;
	};

	struct ComponentTickFunction : public TickFunction
	{
		class ComponentBase* mTarget;

	};

	struct ActorTickFunction : public TickFunction
	{
	public:
		bool mCanEverTick{ true };
		GameObject* mTarget;
	};
}
#endif
