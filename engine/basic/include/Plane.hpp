#ifndef _Plane_H_
#define _Plane_H_
#include "boost/operators.hpp"

namespace Air
{
	template<typename T>
	class Plane_T : boost::equality_comparable<Plane_T<T>>
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

		Plane_T() AIR_NOEXCEPT
		{

		}

		explicit Plane_T(T const * rhs) AIR_NOEXCEPT;
		Plane_T(Plane_T const & rhs) AIR_NOEXCEPT;
		Plane_T(Plane_T && rhs) AIR_NOEXCEPT;
		Plane_T(Vector_T<T, elem_num> const & rhs) AIR_NOEXCEPT;
		Plane_T(Vector_T<T, elem_num> && rhs) AIR_NOEXCEPT;
		Plane_T(T a, T b, T c, T d) AIR_NOEXCEPT;

		iterator begin() AIR_NOEXCEPT
		{
			return mPlane.begin();
		}
		const_iterator begin() const AIR_NOEXCEPT
		{
			return mPlane.begin();
		}

		iterator end() AIR_NOEXCEPT
		{
			return mPlane.end();
		}

		const_iterator end() const AIR_NOEXCEPT
		{
			return mPlane.end();
		}



		reference operator[](size_t index) AIR_NOEXCEPT
		{
			return mPlane[index];
		}
		const_reference operator[](size_t index) const AIR_NOEXCEPT
		{
			return mPlane[index];
		}

		reference a() AIR_NOEXCEPT
		{
			return mPlane[0];
		}
		const_reference a() const AIR_NOEXCEPT
		{
			return mPlane[0];
		}

		reference b() AIR_NOEXCEPT
		{
			return mPlane[1];
		}
		const_reference b() const AIR_NOEXCEPT
		{
			return mPlane[1];
		}

		reference c() AIR_NOEXCEPT
		{
			return mPlane[2];
		}
		const_reference c() const AIR_NOEXCEPT
		{
			return mPlane[2];
		}

		reference d() AIR_NOEXCEPT
		{
			return mPlane[3];
		}
		const_reference d() const AIR_NOEXCEPT
		{
			return mPlane[3];
		}

		Plane_T& operator=(Plane_T const & rhs) AIR_NOEXCEPT;
		Plane_T& operator=(Plane_T && rhs) AIR_NOEXCEPT;
		Plane_T& operator=(Vector_T<T, elem_num> const & rhs) AIR_NOEXCEPT;
		Plane_T& operator=(Vector_T<T, elem_num> &&rhs) AIR_NOEXCEPT;

		Plane_T const operator+() const AIR_NOEXCEPT;
		Plane_T const operator-() const AIR_NOEXCEPT;

		Vector_T<T, 3> const getNormal() const AIR_NOEXCEPT;
		void setNormal(Vector_T<T, 3> const & rhs) AIR_NOEXCEPT;
		bool operator == (Plane_T<T> const & rhs) AIR_NOEXCEPT;



	private:
		Vector_T<T, elem_num> mPlane;
	};
}


#endif