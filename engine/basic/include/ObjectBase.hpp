#ifndef _ObjectBase_H_
#define _ObjectBase_H_
#pragma once
#include "basic/include/ObjectMacros.hpp"
#include "basic/include/Config.h"
namespace Air
{
	class Object;

	class ObjectBase
	{

		friend class ObjectBaseUtility;

	public:

		FORCEINLINE Object* getOuter() const
		{
			return mOuterPrivate;
		}

		virtual ~ObjectBase() {}

		FORCEINLINE EObjectFlags getFlags() const
		{
			return mObjectFlags;
		}

	private:
		EObjectFlags mObjectFlags;

		Object* mOuterPrivate;


	};
}
#endif
