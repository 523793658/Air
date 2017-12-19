#ifndef _TransformComponent_H_
#define _TransformComponent_H_
#pragma once
#include "ComponentBase.hpp"
namespace Air
{
	class TransformComponent : public ComponentBase
	{
	public:
		FORCEINLINE TransformComponent* getAttachParent() const;

	public:
		std::vector<TransformComponent*> mChildren;

		TransformComponent* mAttachParent;
	};
}
#endif
