#include "component/include/TransformComponent.hpp"
namespace Air
{
	FORCEINLINE TransformComponent* TransformComponent::getAttachParent() const
	{
		return mAttachParent;
	}
}