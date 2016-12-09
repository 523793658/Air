#ifndef _Color_H_
#define _Color_H_
#pragma once
#include "basic/include/Basic.h"

#include "FlameMath.h"

namespace Air
{
#ifdef FLAMEMATH
	typedef Color32 Color;
// 	typedef struct _Color {
// 		float r, g, b, a;
// 	}Color;
#else
	template<typename T>
	class Color_T : boost::addable < Color_T<T>,
		boost::subtractable < Color_T<T>,
		boost::dividable2 < Color_T<T>, T,
		boost::multipliable < Color_T<T>,	
		boost::multipliable2<Color_T<T>, T,
		boost::equality_comparable<Color_T<T>> >> >> >
	{
	public:
		enum { elem_num = 4 };

		typedef T value_type;

		typedef typename std::array<T, elem_num>::reference reference;
		typedef typename std::array<T, elem_num>::const_reference const_reference;

		typedef typename std::array<T, elem_num>::iterator iterator;
		typedef typename std::array<T, elem_num>::const_iterator const_iterator;

	public:
		Color_T() AIR_NOEXCEPT
		{
		}
		Color_T(T r, T g, T b, T a) AIR_NOEXCEPT;

		reference r() AIR_NOEXCEPT
		{
			return mValue[0];
		}
		const_reference r() AIR_NOEXCEPT
		{
			return mValue[0];
		}

		reference g() AIR_NOEXCEPT
		{
			return mValue[1];
		}
		const_reference g() AIR_NOEXCEPT
		{
			return mValue[1];
		}

		reference b() AIR_NOEXCEPT
		{
			return mValue[2];
		}
		const_reference b() AIR_NOEXCEPT
		{
			return mValue[2];
		}

		reference a() AIR_NOEXCEPT
		{
			return mValue[3];
		}
		const_reference a() AIR_NOEXCEPT
		{
			return mValue[3];
		}
	private:
		std::array<T, 4> mValue;
	};


#endif // FLAMEMATH
}


#endif