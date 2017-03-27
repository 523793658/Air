#include "basic/include/Basic.h"
#include "basic/include/Quaternion.hpp"
namespace Air
{
	template Quaternion_T<float>::Quaternion_T(float const * rhs) AIR_NOEXCEPT;
	template Quaternion_T<float>::Quaternion_T(float3 const & vec, float s) AIR_NOEXCEPT;
	template Quaternion_T<float>::Quaternion_T(Quaternion const & rhs) AIR_NOEXCEPT;
	template Quaternion_T<float>::Quaternion_T(Quaternion&& rhs) AIR_NOEXCEPT;
	template Quaternion_T<float>::Quaternion_T(float x, float y, float z, float w) AIR_NOEXCEPT;
	template Quaternion const & Quaternion_T<float>::Identity() AIR_NOEXCEPT;
	template Quaternion const & Quaternion_T<float>::operator+=(Quaternion const & rhs) AIR_NOEXCEPT;
	template Quaternion const & Quaternion_T<float>::operator-=(Quaternion const & rhs) AIR_NOEXCEPT;
	template Quaternion const & Quaternion_T<float>::operator*=(Quaternion const & rhs) AIR_NOEXCEPT;
	template Quaternion const & Quaternion_T<float>::operator*=(float rhs) AIR_NOEXCEPT;
	template Quaternion const & Quaternion_T<float>::operator/=(float rhs) AIR_NOEXCEPT;
	template Quaternion& Quaternion_T<float>::operator=(Quaternion const & rhs) AIR_NOEXCEPT;
	template Quaternion& Quaternion_T<float>::operator=(Quaternion&& rhs) AIR_NOEXCEPT;
	template Quaternion const Quaternion_T<float>::operator+() const AIR_NOEXCEPT;
	template Quaternion const Quaternion_T<float>::operator-() const AIR_NOEXCEPT;
	template float3 const Quaternion_T<float>::v() const AIR_NOEXCEPT;
	template void Quaternion_T<float>::v(float3 const & rhs) AIR_NOEXCEPT;
	template bool Quaternion_T<float>::operator==(Quaternion const & rhs) const AIR_NOEXCEPT;


	template <typename T>
	Quaternion_T<T>::Quaternion_T(T const * rhs) AIR_NOEXCEPT
		: mQuat(rhs)
	{
	}

	template <typename T>
	Quaternion_T<T>::Quaternion_T(Vector_T<T, 3> const & vec, T s) AIR_NOEXCEPT
	{
		this->x() = vec.x();
		this->y() = vec.y();
		this->z() = vec.z();
		this->w() = s;
	}

	template <typename T>
	Quaternion_T<T>::Quaternion_T(Quaternion_T const & rhs) AIR_NOEXCEPT
		: mQuat(rhs.mQuat)
	{
	}

	template <typename T>
	Quaternion_T<T>::Quaternion_T(Quaternion_T&& rhs) AIR_NOEXCEPT
		: mQuat(std::move(rhs.mQuat))
	{
	}

	template <typename T>
	Quaternion_T<T>::Quaternion_T(T x, T y, T z, T w) AIR_NOEXCEPT
	{
		this->x() = x;
		this->y() = y;
		this->z() = z;
		this->w() = w;
	}

	template <typename T>
	Quaternion_T<T> const & Quaternion_T<T>::Identity() AIR_NOEXCEPT
	{
		static Quaternion_T const out(0, 0, 0, 1);
		return out;
	}

	template <typename T>
	Quaternion_T<T> const & Quaternion_T<T>::operator+=(Quaternion_T<T> const & rhs) AIR_NOEXCEPT
	{
		mQuat += rhs.mQuat;
		return *this;
	}

	template <typename T>
	Quaternion_T<T> const & Quaternion_T<T>::operator-=(Quaternion_T<T> const & rhs) AIR_NOEXCEPT
	{
		mQuat -= rhs.mQuat;
		return *this;
	}

	template <typename T>
	Quaternion_T<T> const & Quaternion_T<T>::operator*=(Quaternion_T<T> const & rhs) AIR_NOEXCEPT
	{
		*this = MathLib::mul(*this, rhs);
		return *this;
	}

	template <typename T>
	Quaternion_T<T> const & Quaternion_T<T>::operator*=(T rhs) AIR_NOEXCEPT
	{
		mQuat *= static_cast<T>(rhs);
		return *this;
	}

	template <typename T>
	Quaternion_T<T> const & Quaternion_T<T>::operator/=(T rhs) AIR_NOEXCEPT
	{
		mQuat /= static_cast<T>(rhs);
		return *this;
	}

	template <typename T>
	Quaternion_T<T>& Quaternion_T<T>::operator=(Quaternion_T const & rhs) AIR_NOEXCEPT
	{
		if (this != &rhs)
		{
			mQuat = rhs.mQuat;
		}
		return *this;
	}

	template <typename T>
	Quaternion_T<T>& Quaternion_T<T>::operator=(Quaternion_T&& rhs) AIR_NOEXCEPT
	{
		mQuat = std::move(rhs.mQuat);
		return *this;
	}

	template <typename T>
	Quaternion_T<T> const Quaternion_T<T>::operator+() const AIR_NOEXCEPT
	{
		return *this;
	}

	template <typename T>
	Quaternion_T<T> const Quaternion_T<T>::operator-() const AIR_NOEXCEPT
	{
		return Quaternion_T(-this->x(), -this->y(), -this->z(), -this->w());
	}

	template <typename T>
	Vector_T<T, 3> const Quaternion_T<T>::v() const AIR_NOEXCEPT
	{
		return Vector_T<T, 3>(this->x(), this->y(), this->z());
	}

	template <typename T>
	void Quaternion_T<T>::v(Vector_T<T, 3> const & rhs) AIR_NOEXCEPT
	{
		this->x() = rhs.x();
		this->y() = rhs.y();
		this->z() = rhs.z();
	}

	template <typename T>
	bool Quaternion_T<T>::operator==(Quaternion_T<T> const & rhs) const AIR_NOEXCEPT
	{
		return mQuat == rhs.mQuat;
	}



}