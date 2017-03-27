#ifndef _Quaternion_H_
#define _Quaternion_H_
#pragma once
namespace Air
{
	template<typename T>
	class Quaternion_T : boost::addable < Quaternion_T<T>,
		boost::subtractable < Quaternion_T<T>,
		boost::dividable2 < Quaternion_T<T>, T,
		boost::multipliable < Quaternion_T<T>,
		boost::multipliable2<Quaternion_T<T>, T,
		boost::equality_comparable<Quaternion_T<T>> >> >> >
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
		Quaternion_T() AIR_NOEXCEPT
		{
		}
		explicit Quaternion_T(T const * rhs) AIR_NOEXCEPT;
		Quaternion_T(Vector_T<T, 3> const & vec, T s) AIR_NOEXCEPT;
		Quaternion_T(Quaternion_T const & rhs) AIR_NOEXCEPT;
		Quaternion_T(Quaternion_T&& rhs) AIR_NOEXCEPT;
		Quaternion_T(T x, T y, T z, T w) AIR_NOEXCEPT;

		static Quaternion_T const & Identity() AIR_NOEXCEPT;

		// 取向量
		iterator begin() AIR_NOEXCEPT
		{
			return mQuat.begin();
		}
		const_iterator begin() const AIR_NOEXCEPT
		{
			return mQuat.begin();
		}
		iterator end() AIR_NOEXCEPT
		{
			return mQuat.end();
		}
		const_iterator end() const AIR_NOEXCEPT
		{
			return mQuat.end();
		}
		reference operator[](size_t index) AIR_NOEXCEPT
		{
			return mQuat[index];
		}
		const_reference operator[](size_t index) const AIR_NOEXCEPT
		{
			return mQuat[index];
		}

		reference x() AIR_NOEXCEPT
		{
			return mQuat[0];
		}
		const_reference x() const AIR_NOEXCEPT
		{
			return mQuat[0];
		}
		reference y() AIR_NOEXCEPT
		{
			return mQuat[1];
		}
		const_reference y() const AIR_NOEXCEPT
		{
			return mQuat[1];
		}
		reference z() AIR_NOEXCEPT
		{
			return mQuat[2];
		}
		const_reference z() const AIR_NOEXCEPT
		{
			return mQuat[2];
		}
		reference w() AIR_NOEXCEPT
		{
			return mQuat[3];
		}
		const_reference w() const AIR_NOEXCEPT
		{
			return mQuat[3];
		}

		// 赋值操作符
		Quaternion_T const & operator+=(Quaternion_T const & rhs) AIR_NOEXCEPT;
		Quaternion_T const & operator-=(Quaternion_T const & rhs) AIR_NOEXCEPT;

		Quaternion_T const & operator*=(Quaternion_T const & rhs) AIR_NOEXCEPT;
		Quaternion_T const & operator*=(T rhs) AIR_NOEXCEPT;
		Quaternion_T const & operator/=(T rhs) AIR_NOEXCEPT;

		Quaternion_T& operator=(Quaternion_T const & rhs) AIR_NOEXCEPT;
		Quaternion_T& operator=(Quaternion_T&& rhs) AIR_NOEXCEPT;

		// 一元操作符
		Quaternion_T const operator+() const AIR_NOEXCEPT;
		Quaternion_T const operator-() const AIR_NOEXCEPT;

		// 取方向向量
		Vector_T<T, 3> const v() const AIR_NOEXCEPT;
		void v(Vector_T<T, 3> const & rhs) AIR_NOEXCEPT;

		bool operator==(Quaternion_T<T> const & rhs) const AIR_NOEXCEPT;

		private:
			Vector_T<T, elem_num> mQuat;

	};
}
#endif