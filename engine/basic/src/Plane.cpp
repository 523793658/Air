#include "basic/include/Basic.h"
#include "basic/include/Plane.hpp"

namespace Air
{
	template
	Plane::Plane_T(float const * rhs) AIR_NOEXCEPT;
	template Plane::Plane_T(Plane const & rhs) AIR_NOEXCEPT;
	template Plane::Plane_T(Plane && rhs) AIR_NOEXCEPT;
	template Plane::Plane_T(float4 const & rhs) AIR_NOEXCEPT;
	template Plane::Plane_T(float4 && rhs) AIR_NOEXCEPT;
	template Plane::Plane_T(float a, float b, float c, float d) AIR_NOEXCEPT;
	template Plane& Plane::operator=(Plane const & rhs) AIR_NOEXCEPT;
	template Plane& Plane::operator=(Plane && rhs) AIR_NOEXCEPT;
	template Plane& Plane::operator=(float4 const & rhs) AIR_NOEXCEPT;
	template Plane& Plane::operator=(float4 &&rhs) AIR_NOEXCEPT;

	template Plane const Plane::operator+() const AIR_NOEXCEPT;
	template Plane const Plane::operator-() const AIR_NOEXCEPT;

	template float3 const Plane::getNormal() const AIR_NOEXCEPT;
	template void Plane::setNormal(float3 const & rhs) AIR_NOEXCEPT;
	template bool Plane::operator == (Plane const & rhs) AIR_NOEXCEPT;



	template <typename T>
	Plane_T<T>::Plane_T(T const * rhs) AIR_NOEXCEPT
		:mPlane(rhs)
	{

	}

	template <typename T>
	Plane_T<T>::Plane_T(Plane_T const & rhs) AIR_NOEXCEPT
		: mPlane(rhs.mPlane)
	{

	}

	template <typename T>
	Plane_T<T>::Plane_T(Plane_T && rhs) AIR_NOEXCEPT
		:mPlane(std::move(rhs.mPlane))
	{

	}

	template <typename T>
	Plane_T<T>::Plane_T(Vector_T<T, elem_num> const & rhs) AIR_NOEXCEPT
		:mPlane(rhs)
	{
		
	}

	template <typename T>
	Plane_T<T>::Plane_T(Vector_T<T, elem_num> && rhs) AIR_NOEXCEPT
		:mPlane(std::move(rhs))
	{

	}

	template <typename T>
	Plane_T<T>::Plane_T(T a, T b, T c, T d) AIR_NOEXCEPT
		:mPlane(a, b, c, d)
	{

	}

	template <typename T>
	Plane_T<T>& Plane_T<T>::operator=(Plane_T<T> const & rhs) AIR_NOEXCEPT
	{
		if (&rhs != this)
		{
			mPlane = rhs.mPlane;
		}
		return *this;
	}

	template <typename T>
	Plane_T<T>& Plane_T<T>::operator=(Plane_T<T> && rhs) AIR_NOEXCEPT
	{
		mPlane = std::move(rhs.mPlane);
		return *this;
	}

	template <typename T>
	Plane_T<T>& Plane_T<T>::operator=(Vector_T<T, elem_num> const & rhs) AIR_NOEXCEPT
	{
		mPlane = rhs;
		return *this;
	}

	template <typename T>
	Plane_T<T>& Plane_T<T>::operator=(Vector_T<T, elem_num> &&rhs) AIR_NOEXCEPT
	{
		mPlane = std::move(rhs);
		return *this;
	}

	template <typename T>
	Plane_T<T> const Plane_T<T>::operator+() const AIR_NOEXCEPT
	{
		return *this;
	}

	template <typename T>
	Plane_T<T> const Plane_T<T>::operator-() const AIR_NOEXCEPT
	{
		Plane_T<T> temp(-mPlane);
		return temp;
	}

	template <typename T>
	Vector_T<T, 3> const Plane_T<T>::getNormal() const AIR_NOEXCEPT
	{
		return Vector_T<T, 3>(this->a(), this->b(), this->c());
	}

	template <typename T>
	void Plane_T<T>::setNormal(Vector_T<T, 3> const & rhs) AIR_NOEXCEPT
	{
		this->a() = rhs.x();
		this->b() = rhs.y();
		this->c() = rhs.z();
	}

	template <typename T>
	bool Plane_T<T>::operator == (Plane_T<T> const & rhs) AIR_NOEXCEPT
	{
		return mPlane == rhs.mPlane;
	}
}