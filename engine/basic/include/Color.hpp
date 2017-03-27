#ifndef _Color_H_
#define _Color_H_
#pragma once
#include "basic/include/Basic.h"
namespace Air
{
	template <typename T>
	class Color_T : boost::addable < Color_T<T>,
		boost::subtractable < Color_T<T>,
		boost::dividable2 < Color_T<T>, T,
		boost::multipliable < Color_T<T>,
		boost::multipliable2<Color_T<T>, T,
		boost::equality_comparable<Color_T<T>> >> >> >
	{
	public:	
		enum {elem_num = 4};
		typedef T value_type;
		typedef typename Vector_T<T, elem_num>::pointer pointer;
		typedef typename Vector_T<T, elem_num>::const_pointer const_pointer;
		typedef typename Vector_T<T, elem_num>::reference reference;
		typedef typename Vector_T<T, elem_num>::const_reference const_reference;
		typedef typename Vector_T<T, elem_num>::iterator iterator;
		typedef typename Vector_T<T, elem_num>::const_iterator const_iterator;

	public:
		Color_T() AIR_NOEXCEPT
		{

		}
		explicit Color_T(T const * rhs) AIR_NOEXCEPT;
		Color_T(Color_T const & rhs) AIR_NOEXCEPT;
		Color_T(Color_T && rhs) AIR_NOEXCEPT;
		Color_T(T r, T g, T b, T a) AIR_NOEXCEPT;
		explicit Color_T(uint32_t dw) AIR_NOEXCEPT;

		iterator begin() AIR_NOEXCEPT
		{
			return mCol.begin();
		}

		const_iterator begin() const AIR_NOEXCEPT
		{
			return mCol.begin();
		}

		iterator end() AIR_NOEXCEPT
		{
			return mCol.end();
		}

		const_iterator end() const AIR_NOEXCEPT
		{
			return mCol.end();
		}

		reference operator[](size_t index) AIR_NOEXCEPT
		{
			return mCol[index];
		}

		const_reference operator[](size_t index) const AIR_NOEXCEPT
		{
			return mCol[index];
		}

		reference r() AIR_NOEXCEPT
		{
			return mCol[0];
		}
		const_reference r() const AIR_NOEXCEPT
		{
			return mCol[0];
		}

		reference g() AIR_NOEXCEPT
		{
			return mCol[1];
		}
		const_reference g() const AIR_NOEXCEPT
		{
			return mCol[1];
		}
		reference b() AIR_NOEXCEPT
		{
			return mCol[2];
		}
		const_reference b() const AIR_NOEXCEPT
		{
			return mCol[2];
		}
		reference a() AIR_NOEXCEPT
		{
			return mCol[3];
		}
		const_reference a() const AIR_NOEXCEPT
		{
			return mCol[3];
		}

		void getRGBA(uint8_t& R, uint8_t& g, uint8_t& b, uint8_t& a) const AIR_NOEXCEPT;

		uint32_t getARGB() const AIR_NOEXCEPT;
		uint32_t getABGR() const AIR_NOEXCEPT;

		Color_T& operator +=(Color_T<T> const & rhs) AIR_NOEXCEPT;
		Color_T& operator -=(Color_T<T> const & rhs) AIR_NOEXCEPT;
		Color_T& operator *=(T rhs) AIR_NOEXCEPT;
		Color_T& operator *=(Color_T<T> const & rhs) AIR_NOEXCEPT;
		Color_T& operator /=(T rhs) AIR_NOEXCEPT;

		Color_T& operator = (Color_T const & rhs) AIR_NOEXCEPT;
		Color_T& operator = (Color_T && rhs) AIR_NOEXCEPT;

		Color_T const operator +() const AIR_NOEXCEPT;
		Color_T const operator -() const AIR_NOEXCEPT;

		bool operator==(Color_T<T> const & rhs) const AIR_NOEXCEPT;

	private:
		Vector_T<T, elem_num> mCol;
	};
}


#endif