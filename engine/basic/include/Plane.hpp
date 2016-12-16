#ifndef _Plane_H_
#define _Plane_H_
#include "boost/operators.hpp"

namespace Air
{
#ifdef FLAMEMATH
	typedef Plane Plane;
#else
	class Plane : boost::equality_comparable<Plane>
	{
	public:
		Plane() AIR_NOEXCEPT
		{

		}

		explicit Plane(float const * rhs) AIR_NOEXCEPT;
		Plane(Plane const & rhs) AIR_NOEXCEPT;
		Plane(Plane && rhs) AIR_NOEXCEPT;
		Plane(float4 const & rhs) AIR_NOEXCEPT;
		Plane(float4 && rhs) AIR_NOEXCEPT;
		Plane(float a, float b, float c, float d) AIR_NOEXCEPT;

		float& operator[](size_t index) AIR_NOEXCEPT
		{
			return mPlane[index];
		}
		float& const operator[](size_t index) AIR_NOEXCEPT
		{
			return mPlane[index];
		}
	private:
		float mPlane[4];
	};
#endif // FLAMEMATH
}


#endif