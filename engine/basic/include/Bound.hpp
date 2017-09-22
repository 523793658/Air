#ifndef _Bound_H_
#define _Bound_H_
#pragma once
#include "basic/include/PreDeclare.h"
namespace Air
{
	template<typename T>
	class Bound_T
	{
	public:
		virtual~Bound_T() AIR_NOEXCEPT
		{

		}
		virtual bool isEmpty() const AIR_NOEXCEPT = 0;
		virtual bool contains(Vector_T<T, 3> const & v) const AIR_NOEXCEPT = 0;
		virtual T getMaxRadiusSq() const AIR_NOEXCEPT = 0;
	};

}



#endif