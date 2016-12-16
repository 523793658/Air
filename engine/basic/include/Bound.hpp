#ifndef _Bound_H_
#define _Bound_H_
#pragma once

namespace Air
{
	class Bound_T
	{
	public:
		virtual~Bound_T() AIR_NOEXCEPT
		{

		}
		virtual bool isEmpty() const AIR_NOEXCEPT = 0;
		virtual bool verInBound(float3 const & v) const AIR_NOEXCEPT = 0;
		virtual float getMaxRadiusSq() const AIR_NOEXCEPT = 0;
	};

}



#endif