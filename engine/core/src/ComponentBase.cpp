#include "Context.h"
#include "core/include/World.hpp"
#include "core/include/GameObject.hpp"

#include "ComponentBase.hpp"

namespace Air
{
	class GameObject* ComponentBase::getOwner() const
	{
		return mOwner;
	}

	void ComponentBase::activate(bool bReset /* = false */)
	{

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
}