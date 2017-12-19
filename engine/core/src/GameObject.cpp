#include "Context.h"
#include "World.hpp"
#include "GameObject.hpp"
#include "Level.hpp"
#include <set>
namespace Air
{
	static TransformComponent* getUnregisteredParent(ComponentBase* component)
	{
		TransformComponent* parentComponent = nullptr;
		TransformComponent* transformComponent = dynamic_cast<TransformComponent*>(component);
		while (transformComponent &&
			transformComponent->getAttachParent() &&
			!transformComponent->getAttachParent()->isRegistered())
		{
			transformComponent = transformComponent->getAttachParent();
			if (transformComponent->mAutoRegister && !transformComponent->isPendingKill())
			{
				parentComponent = transformComponent;
			}
		}
		return parentComponent;
	}

	GameObject::GameObject()
	{
		mTransform = new TransformComponent();
		mOwnedComponents.emplace(mTransform);
	}
	GameObject::GameObject(std::string name)
		:mName(name)
	{
		mTransform = new TransformComponent();
		mOwnedComponents.emplace(mTransform);
	}

	bool GameObject::isInitialized() const
	{
		return mInitialized;
	}

	Level* GameObject::getLevel() const
	{
		return dynamic_cast<Level*>(getOuter());
	}

	World* GameObject::getWorld() const
	{
		if (!hasAnyFlags(RF_ClassDefaultObject) && !getOuter()->hasAnyFlags(RF_BeginDestroyed))
		{
			if (Level* level = getLevel())
			{
				return level->mOwningWorld;
			}
		}
		return nullptr;
	}

	void GameObject::registerAllActorTickFunctions(bool bRegister, bool bDoComponents)
	{
		if (mTickFunctionsRegistered != bRegister)
		{
			registerActorTickFunctions(bRegister);
		}
		if (bDoComponents)
		{
			for (auto component : getComponents())
			{
				if (component)
				{
					component->registerAllComponentTickFunctions(bRegister);
				}
			}
		}
	}

	bool GameObject::incrementalRegisterComponents(int32_t numComponentsToRegister)
	{
		if (numComponentsToRegister == 0)
		{
			numComponentsToRegister = std::numeric_limits<int32_t>::max();
		}
		World* const world = getWorld();
		BOOST_ASSERT(world != nullptr);
		if (mAllowTickBeforeBeginPlay || !world->isGameWorld())
		{
			registerAllActorTickFunctions(true, false);
		}

		if (!mTransform->isRegistered())
		{
			if (mTransform->mAutoRegister)
			{
				mTransform->modify(false);
				mTransform->registerComponentWithWorld(world);
			}
		}


		int32_t numTotalRegisteredComponents = 0;
		int32_t numRegisterComponentThisRun = 0;

		auto _RegisterComponent = [&world, &numRegisterComponentThisRun](ComponentBase* cp, bool modify)
		{
			cp->modify(modify);
			cp->registerComponentWithWorld(world);
			numRegisterComponentThisRun++;
		};
		

	
		std::vector<ComponentBase*> components;
		getComponents(components);
		std::set<ComponentBase*> registeredParent;
		for (int32_t compIdx = 0; compIdx < components.size() && numRegisterComponentThisRun < numComponentsToRegister; ++compIdx)
		{
			ComponentBase* component = components[compIdx];
			if (!component->isRegistered() && component->mAutoRegister && !component->isPendingKill())
			{
				TransformComponent* unregisteredParentComponent = getUnregisteredParent(component);
				if (unregisteredParentComponent)
				{
					if (!registeredParent.emplace(unregisteredParentComponent).second)
					{
						break;
					}
					component = unregisteredParentComponent;
					compIdx--;
					numTotalRegisteredComponents--;
				}
				component->modify(false);
				component->registerComponentWithWorld(world);
				numRegisterComponentThisRun++;
			}
			numTotalRegisteredComponents++;
		}
		if (components.size() == numTotalRegisteredComponents)
		{
			postRegisterAllComponents();
			return true;
		}
		return false;
	}

	void GameObject::postRegisterAllComponents()
	{

	}

	bool GameObject::ownsComponent(ComponentBase* component) const
	{
		return mOwnedComponents.find(component) != mOwnedComponents.end();
	}
	bool GameObject::isActorInitialized() const
	{
		return mInitialized;
	}

	bool GameObject::hasActorBegunPlay() const
	{
		return mActorHasBegunPlay == EActorBeginPlayState::HasBegunPlay;
	}

	bool GameObject::isActorBeginningPlay() const
	{
		return mActorHasBegunPlay == EActorBeginPlayState::BeginningPlay;
	}

	void GameObject::registerActorTickFunctions(bool bRegister)
	{
		if (bRegister)
		{
			if (mPrimaryActorTick.mCanEverTick)
			{
				mPrimaryActorTick.mTarget = this;
				mPrimaryActorTick.setTickFunctionEnable(mPrimaryActorTick.mStartWithTickEnabled || mPrimaryActorTick.isTickFunctionEnabled());
			}
		}
		else
		{
			if (mPrimaryActorTick.isTickFunctionRegistered())
			{
				mPrimaryActorTick.unRegisterTickFunction();
			}
		}

	}


}