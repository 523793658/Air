#ifndef _GameObject_H_
#define _GameObject_H_
#pragma once
#include "PreDeclare.h"
#include "Type.hpp"
#include "component/include/TransformComponent.hpp"
#include <set>
namespace Air
{
	class World;
	class Level;
	class AIR_CORE_API GameObject : public Object
	{
	public:
		GameObject();
		GameObject(std::string name);

		bool isInitialized() const;

		bool incrementalRegisterComponents(int32_t numComponentsToRegister = 0);

		World* getWorld() const;

		Level* getLevel() const;

		void registerAllActorTickFunctions(bool bRegister, bool bDoComponents);

		template<class T>
		void getComponents(std::vector<T*>& outComponents, bool bIncludeFromChildObjects = false) const
		{
			static_assert(std::is_convertible<T, const ComponentBase>::value, "'T' template parameter to getComponents must be derived from ComponentBase");
			outComponents.reserve(mOwnedComponents.size());
			outComponents.clear();
			for (auto * ownedComponent : mOwnedComponents)
			{
				if (T* component = dynamic_cast<T*>(ownedComponent))
				{
					outComponents.push_back(component);
				}
			}
			if (bIncludeFromChildObjects)
			{
				std::vector<ComponentBase*> componentsInChild;
				for (TransformComponent* child : mTransform->mChildren)
				{
					child->getOwner()->getComponents(componentsInChild, true);
					outComponents.insert(outComponents.end(), componentsInChild.begin(), componentsInChild.end());
				}
			}
		}

		const std::set<ComponentBase*>& getComponents() const
		{
			return mOwnedComponents;
		}


		void postRegisterAllComponents();

		bool ownsComponent(ComponentBase* component) const;

		bool isActorInitialized() const;

		bool hasActorBegunPlay() const;

		bool isActorBeginningPlay() const;

		virtual void registerActorTickFunctions(bool bRegister);


	public:
		TransformComponent* mTransform;
	private:
		std::string mName;


		bool mInitialized{ false };

		bool mAllowTickBeforeBeginPlay{ false };

		bool mTickFunctionsRegistered{ true };

		std::set<ComponentBase*> mOwnedComponents;

		struct ActorTickFunction mPrimaryActorTick;

		enum class EActorBeginPlayState : uint8_t
		{
			HasNotBegunPlay,
			BeginningPlay,
			HasBegunPlay,
		};

		EActorBeginPlayState mActorHasBegunPlay{ EActorBeginPlayState::HasBegunPlay };
	};





}





#endif
