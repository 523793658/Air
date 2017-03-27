#include "basic/include/Basic.h"
#include "basic/include/Vector.hpp"
#include "basic/include/Matrix.hpp"
namespace Air
{
	template float4x4::Matrix4_T(float const * rhs) AIR_NOEXCEPT;
	template float4x4::Matrix4_T( float4x4 const & rhs) AIR_NOEXCEPT;
	template float4x4::Matrix4_T(float4x4 && rhs) AIR_NOEXCEPT;
	template float4x4::Matrix4_T(float f11, float f12, float f13, float f14,
		float f21, float f22, float f23, float f24,
		float f31, float f32, float f33, float f34,
		float f41, float f42, float f43, float f44
		) AIR_NOEXCEPT;

	template float4x4 float4x4::createLookAtLH(float3 const & vEye, float3 const & vAt, float3 const & vUp) AIR_NOEXCEPT;
	template float4x4 const & float4x4::zero() AIR_NOEXCEPT;
	template float4x4 const & float4x4::identify() AIR_NOEXCEPT;
	template float4x4 float4x4::createLookAtLH(float3 const & vEye, float3 const & vAt, float3 const & vUp) AIR_NOEXCEPT;
	template float4x4 float4x4::createPerspectiveFOVLH(float const & fov, float const & aspect, float const & nearPlane, float const & farPlane) AIR_NOEXCEPT;
	template float4x4 float4x4::createOrthoLH(float const & w, float const & h, float const & nearPlane, float const & farPlane) AIR_NOEXCEPT;
	template float4x4 float4x4::createOrthoCenterLH(float const & left, float const & right, float const & bottom, float const & top, float const & near_plane, float const & far_plane) AIR_NOEXCEPT;
	template void float4x4::row(size_t index, float4 const & rhs) AIR_NOEXCEPT;
	template float4 const & float4x4::row(size_t index) const AIR_NOEXCEPT;

	template void float4x4::col(size_t index, float4 const & rhs) AIR_NOEXCEPT;
	template float4 const float4x4::col(size_t index) const AIR_NOEXCEPT;

	template float4x4& float4x4::operator +=(float4x4 const & rhs) AIR_NOEXCEPT;
	template float4x4& float4x4::operator -=(float4x4 const & rhs) AIR_NOEXCEPT;
	template float4x4& float4x4::operator *=(float4x4 const & rhs) AIR_NOEXCEPT;

	template float4x4& float4x4::operator *=(float rhs) AIR_NOEXCEPT;
	template float4x4& float4x4::operator /=(float rhs) AIR_NOEXCEPT;

	template float4x4& float4x4::operator = (float4x4 const & rhs) AIR_NOEXCEPT;
	template float4x4& float4x4::operator = (float4x4 && rhs) AIR_NOEXCEPT;

	template float4x4 const float4x4::operator +() const AIR_NOEXCEPT;
	template float4x4 const float4x4::operator -() const AIR_NOEXCEPT;

	template bool float4x4::operator == (float4x4 const & rhs) const AIR_NOEXCEPT;

	template float4x4 const float4x4::inverse() const AIR_NOEXCEPT;








	template<typename T>
	Matrix4_T<T>::Matrix4_T(T const *rhs) AIR_NOEXCEPT
	{
		for (size_t i = 0; i < row_num; ++i)
		{
			m[i] = Vector_T<T, col_num>(rhs);
			rhs += col_num;
		}
	}

	template<typename T>
	Matrix4_T<T>::Matrix4_T(Matrix4_T const & rhs) AIR_NOEXCEPT
		:m(rhs.m)
	{

	}
	template<typename T>
	Matrix4_T<T>::Matrix4_T(Matrix4_T && rhs) AIR_NOEXCEPT
		:m(std::move(rhs.m))
	{

	}
	template<typename T>

	Matrix4_T<T>::Matrix4_T(T f11, T f12, T f13, T f14,
		T f21, T f22, T f23, T f24,
		T f31, T f32, T f33, T f34,
		T f41, T f42, T f43, T f44
		) AIR_NOEXCEPT
	{
		m[0][0] = f11; m[0][1] = f12; m[0][2] = f13; m[0][3] = f14;
		m[1][0] = f21; m[1][1] = f22; m[1][2] = f23; m[1][3] = f24;
		m[2][0] = f31; m[2][1] = f32; m[2][2] = f33; m[2][3] = f34;
		m[3][0] = f41; m[3][1] = f42; m[3][2] = f43; m[3][3] = f44;
	}

	template<typename T>
	Matrix4_T<T> const & Matrix4_T<T>::zero() AIR_NOEXCEPT
	{
		static Matrix4_T const out(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0);
		return out;
	}

	template<typename T>
	Matrix4_T<T> const & Matrix4_T<T>::identify() AIR_NOEXCEPT
	{
		static Matrix4_T const out(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
		return out;
	}

	template<typename T>
	void Matrix4_T<T>::row(size_t index, Vector_T<T, 4> const & rhs) AIR_NOEXCEPT
	{
		m[index] = rhs;
	}

	template<typename T>
	Vector_T<T, 4> const & Matrix4_T<T>::row(size_t index) const AIR_NOEXCEPT
	{
		return m[index];
	}

	template<typename T>
	void Matrix4_T<T>::col(size_t index, Vector_T<T, 4> const & rhs) AIR_NOEXCEPT
	{
		for (size_t i = 0; i < row_num; ++i)
		{
			m[i][index] = rhs[i];
		}
	}

	template<typename T>
	Vector_T<T, 4> const Matrix4_T<T>::col(size_t index) const AIR_NOEXCEPT
	{
		Vector_T<T, 4> ret;
		for (size_t i = 0; i < row_num; ++i)
		{
			ret[i] = m[i][index];
		}
		return ret;
	}
	template<typename T>
	Matrix4_T<T>& Matrix4_T<T>::operator+=(Matrix4_T const & rhs) AIR_NOEXCEPT
	{
		m += rhs.m;
		return *this;
	}

	template<typename T>
	Matrix4_T<T>& Matrix4_T<T>::operator -=(Matrix4_T const & rhs) AIR_NOEXCEPT
	{
		m -= rhs.m;
		return *this;
	}

	template<typename T>
	Matrix4_T<T>& Matrix4_T<T>::operator *=(Matrix4_T const & rhs) AIR_NOEXCEPT
	{
		*this = MathLib::mul(*this, rhs);
		return *this;
	}

	template<typename T>
	Matrix4_T<T>& Matrix4_T<T>::operator *=(T rhs) AIR_NOEXCEPT
	{
		for (size_t i = 0; i < row_num; ++i)
		{
			m[i] *= rhs;
		}
		return *this;
	}

	template<typename T>
	Matrix4_T<T>& Matrix4_T<T>::operator /=(T rhs) AIR_NOEXCEPT
	{
		return this->operator *=(1 / rhs);
	}


	template<typename T>
	Matrix4_T<T>& Matrix4_T<T>::operator = (Matrix4_T const & rhs) AIR_NOEXCEPT
	{
		if (this != &rhs)
		{
			m = rhs.m;
		}
		return *this;
	}

	template<typename T>
	Matrix4_T<T>& Matrix4_T<T>::operator = (Matrix4_T && rhs) AIR_NOEXCEPT
	{
		m = std::move(rhs.m);
		return *this;
	}
	template<typename T>
	Matrix4_T<T> const Matrix4_T<T>::operator +() const AIR_NOEXCEPT
	{
		return *this;
	}

	template<typename T>
	Matrix4_T<T> const Matrix4_T<T>::operator -() const AIR_NOEXCEPT
	{
		Matrix4_T<T> temp;
		temp.m = -m;
		return temp;
	}

	template<typename T>
	bool Matrix4_T<T>::operator == (Matrix4_T const & rhs) const AIR_NOEXCEPT
	{
		return m == rhs.m;
	}

	template<typename T>
	Matrix4_T<T> Matrix4_T<T>::createLookAtLH(float3 const & vEye, float3 const & vAt, float3 const & vUp) AIR_NOEXCEPT
	{
		return MathLib::look_at_lh(vEye, vAt, vUp);
	}

	template<typename T>
	Matrix4_T<T> Matrix4_T<T>::createPerspectiveFOVLH(float const & fov, float const & aspect, float const & nearPlane, float const & farPlane) AIR_NOEXCEPT
	{
		return MathLib::perspective_fov_lh(fov, aspect, nearPlane, farPlane);
	}
	
	template<typename T>
	Matrix4_T<T> Matrix4_T<T>::createOrthoLH(float const & w, float const & h, float const & nearPlane, float const & farPlane) AIR_NOEXCEPT
	{
		return MathLib::ortho_lh(w, h, nearPlane, farPlane);
	}

	template<typename T>
	Matrix4_T<T> Matrix4_T<T>::createOrthoCenterLH(float const & left, float const & right, float const & bottom, float const & top, float const & near_plane, float const & far_plane) AIR_NOEXCEPT
	{
		return MathLib::ortho_off_center_lh(left, right, bottom, top, near_plane, far_plane);
	}

	template<typename T>
	Matrix4_T<T> const Matrix4_T<T>::inverse() const AIR_NOEXCEPT
	{
		return MathLib::inverse(*this);
	}

}