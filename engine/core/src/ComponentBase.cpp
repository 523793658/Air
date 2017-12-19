#include "Context.h"
#include "core/include/World.hpp"
#include "core/include/GameObject.hpp"

#include "ComponentBase.hpp"

namespace Air
{
	ComponentBase::ComponentBase()
	{

	}

	class GameObject* ComponentBase::getOwner() const
	{
		return mOwner;
	}

	void ComponentBase::activate(bool bReset /* = false */)
	{

	}

	bool ComponentBase::modify(bool bAlwaysMarkDirty)
	{
		return true;
	}

	void ComponentBase::onRegister()
	{
		mRegistered = true;

		updateComponentToWorld();

		if (mAutoActivate)
		{
			GameObject* owner = getOwner();
			if (!mWorldPrivate->isGameWorld() || owner == nullptr || owner->isInitialized())
			{
				activate(true);
			}
		}
	}

	void ComponentBase::onUnRegister()
	{
		mRegistered = false;
	}

	void ComponentBase::registerComponentWithWorld(World* inWorld)
	{
		if (isRegistered())
		{
			return;
		}
		if (inWorld == nullptr)
		{
			return;
		}
		BOOST_ASSERT(mWorldPrivate == nullptr, getFullName());
		GameObject* myOwner = getOwner();
		//BOOST_ASSERT(myOwner != nullptr && myOwner->ownsComponent(this));
		if (!mHasBeenCreated)
		{
			onComponentCreated();
		}
		mWorldPrivate = inWorld;
		executeRegisterEvents();

		if (!inWorld->isGameWorld())
		{
			registerAllComponentTickFunctions(true);
		}
		else if(myOwner == nullptr)
		{
			if (!mHasBeenInitialized && mWantsInitializeComponent)
			{
				initializeComponent();
			}
			registerAllComponentTickFunctions(true);
		}
		else
		{
			if (!mHasBeenInitialized && mWantsInitializeComponent && myOwner->isActorInitialized())
			{
				initializeComponent();
			}
			if (myOwner->hasActorBegunPlay())
			{
				initializeComponent();
			}
			if (myOwner->hasActorBegunPlay() || myOwner->isActorBeginningPlay())
			{
				registerAllComponentTickFunctions(true);
				if (!mHasBegunPlay)
				{
					beginPlay();
				}
			}
		}
	}

	static ComponentBase* GTestRegisterComponentTickFunction = nullptr;


	void ComponentBase::registerComponentTickFunctions(bool bRegister)
	{
		if (bRegister)
		{
			if (setupComponentTickFunction(&mPrimaryComponentTick))
			{
				mPrimaryComponentTick.mTarget = this;
			}
		}
		else
		{
			if (mPrimaryComponentTick.isTickFunctionRegistered())
			{
				mPrimaryComponentTick.unRegisterTickFunction();
			}
		}
		GTestRegisterComponentTickFunction = this;
	}

	void ComponentBase::registerAllComponentTickFunctions(bool bRegister)
	{
		BOOST_ASSERT(GTestRegisterComponentTickFunction == nullptr);
		if (bRegister)
		{
			if (mTickFunctionsRegistered != bRegister)
			{
				registerComponentTickFunctions(bRegister);
				mTickFunctionsRegistered = bRegister;
				GTestRegisterComponentTickFunction = nullptr;
			}
		}
	}
}