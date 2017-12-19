#ifndef _ObjectBaseUtility_H_
#define _ObjectBaseUtility_H_
#pragma once
#include "basic/include/ObjectBase.hpp"
#include "basic/include/ObjectMacros.hpp"
namespace Air
{
	class ObjectBaseUtility : public ObjectBase
	{
	public:
		virtual ~ObjectBaseUtility(){}

		FORCEINLINE bool hasAnyFlags(EObjectFlags flagsToCheck) const
		{
			return (getFlags() & flagsToCheck) != 0;
		}

		FORCEINLINE bool isPendingKill() const
		{
			return false;
		}
	};
}
#endif
