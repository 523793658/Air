#ifndef _Matrix_H_
#define _Matrix_H_
#pragma once

#include "boost/operators.hpp"

namespace Air
{
	template <typename T>
	class Matrix4_T : boost::addable < Matrix4_T<T>,
		boost::subtractable < Matrix4_T<T>,
		boost::multipliable2 < Matrix4_T<T>, T,
		boost::dividable2 < Matrix4_T<T>, T,
		boost::multipliable<Matrix4_T<T>,
		boost::equality_comparable<Matrix4_T<T>> >> >> >
	{
	public:
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef value_type const *	const_pointer;
		typedef value_type&			reference;
		typedef value_type const & const_reference;

		typedef value_type*			iterator;
		typedef value_type const *	const_iterator;

		enum { row_num = 4, col_num = 4 };
		enum { elem_num = row_num * col_num };
	public:
		Matrix4_T() AIR_NOEXCEPT
		{

		}
		explicit Matrix4_T(T const *rhs) AIR_NOEXCEPT;
		Matrix4_T(Matrix4_T const & rhs) AIR_NOEXCEPT;
		Matrix4_T(Matrix4_T && rhs) AIR_NOEXCEPT;
		Matrix4_T(T f11, T f12, T f13, T f14,
			T f21, T f22, T f23, T f24,
			T f31, T f32, T f33, T f34,
			T f41, T f42, T f43, T f44
			) AIR_NOEXCEPT;

		static size_t size() AIR_NOEXCEPT
		{
			return elem_num;
		}

		static Matrix4_T const & zero() AIR_NOEXCEPT;
		static Matrix4_T const & identify() AIR_NOEXCEPT;
		static Matrix4_T createLookAtLH(float3 const & vEye, float3 const & vAt, float3 const & vUp) AIR_NOEXCEPT;
		static Matrix4_T createPerspectiveFOVLH(float const & fov, float const & aspect, float const & nearPlane, float const & farPlane) AIR_NOEXCEPT;
		static Matrix4_T createOrthoLH(float const & w, float const & h, float const & nearPlane, float const & farPlane) AIR_NOEXCEPT;
		static Matrix4_T createOrthoCenterLH(float const & left, float const & right, float const & bottom, float const & top, float const & near_plane, float const & far_plane) AIR_NOEXCEPT;


		reference operator()(size_t row, size_t col) AIR_NOEXCEPT
		{
			return m[row][col];
		}
		const_reference operator()(size_t row, size_t col) const AIR_NOEXCEPT
		{
			return m[row][col];
		}

		iterator begin() AIR_NOEXCEPT
		{
			return &m[0][0];
		}
		const_iterator begin() const AIR_NOEXCEPT
		{
			return &m[0][0];
		}

		iterator end() AIR_NOEXCEPT
		{
			return &m[0][0] + elem_num;
		}
		const_iterator end() const AIR_NOEXCEPT
		{
			return &m[0][0] + elem_num;
		}

		reference operator[](size_t index) AIR_NOEXCEPT
		{
			return *(this->begin() + index);
		}
		const_reference operator[](size_t index) const AIR_NOEXCEPT
		{
			return *(this->begin() + index);
		}

		void row(size_t index, Vector_T<T, 4> const & rhs) AIR_NOEXCEPT;
		Vector_T<T, 4> const & row(size_t index) const AIR_NOEXCEPT;

		void col(size_t index, Vector_T<T, 4> const & rhs) AIR_NOEXCEPT;
		Vector_T<T, 4> const col(size_t index) const AIR_NOEXCEPT;

		Matrix4_T& operator +=(Matrix4_T const & rhs) AIR_NOEXCEPT;
		Matrix4_T& operator -=(Matrix4_T const & rhs) AIR_NOEXCEPT;
		Matrix4_T& operator *=(Matrix4_T const & rhs) AIR_NOEXCEPT;

		Matrix4_T& operator *=(T rhs) AIR_NOEXCEPT;
		Matrix4_T& operator /=(T rhs) AIR_NOEXCEPT;

		Matrix4_T& operator = (Matrix4_T const & rhs) AIR_NOEXCEPT;
		Matrix4_T& operator = (Matrix4_T && rhs) AIR_NOEXCEPT;

		Matrix4_T const operator +() const AIR_NOEXCEPT;
		Matrix4_T const operator -() const AIR_NOEXCEPT;

		bool operator == (Matrix4_T const & rhs) const AIR_NOEXCEPT;

		Matrix4_T const inverse() const AIR_NOEXCEPT;

	private:
		Vector_T<Vector_T<T, col_num>, row_num> m;
	};
}


#endif