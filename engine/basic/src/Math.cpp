#include "basic/include/Basic.h"
#include "basic/include/Math.hpp"
namespace Air
{
	namespace MathLib
	{
		float abs(float x) AIR_NOEXCEPT
		{
			union FNI
			{
				float f;
				int32_t i;
			} fni;
			fni.f = x;
			fni.i &= 0x7FFFFFFF;
			return fni.f;
		}

		float sqrt(float x) AIR_NOEXCEPT
		{
			return std::sqrt(x);
		}

		// From Quake III. But the magic number is from http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf
		float recip_sqrt(float number) AIR_NOEXCEPT
		{
			float const threehalfs = 1.5f;

			float x2 = number * 0.5f;
			union FNI
			{
				float f;
				int32_t i;
			} fni;
			fni.f = number;											// evil floating point bit level hacking
			fni.i = 0x5f375a86 - (fni.i >> 1);						// what the fuck?
			fni.f = fni.f * (threehalfs - (x2 * fni.f * fni.f));	// 1st iteration
			fni.f = fni.f * (threehalfs - (x2 * fni.f * fni.f));		// 2nd iteration, this can be removed

			return fni.f;
		}

		float pow(float x, float y) AIR_NOEXCEPT
		{
			return std::pow(x, y);
		}

		float exp(float x) AIR_NOEXCEPT
		{
			return std::exp(x);
		}

		float log(float x) AIR_NOEXCEPT
		{
			return std::log(x);
		}

		float log10(float x) AIR_NOEXCEPT
		{
			return std::log10(x);
		}

		float sin(float x) AIR_NOEXCEPT
		{
			return std::sin(x);
		}

		float cos(float x) AIR_NOEXCEPT
		{
			return sin(x + PI / 2);
		}

		void sincos(float x, float& s, float& c) AIR_NOEXCEPT
		{
			s = sin(x);
			c = cos(x);
		}

		float tan(float x) AIR_NOEXCEPT
		{
			return std::tan(x);
		}

		float asin(float x) AIR_NOEXCEPT
		{
			return std::asin(x);
		}

		float acos(float x) AIR_NOEXCEPT
		{
			return std::acos(x);
		}

		float atan(float x) AIR_NOEXCEPT
		{
			return std::atan(x);
		}

		float sinh(float x) AIR_NOEXCEPT
		{
			return std::sinh(x);
		}

		float cosh(float x) AIR_NOEXCEPT
		{
			return std::cosh(x);
		}

		float tanh(float x) AIR_NOEXCEPT
		{
			return std::tanh(x);
		}
	}
}