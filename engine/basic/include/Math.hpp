#ifndef _Basic_Math_H_
#define _Basic_Math_H_
#pragma once
#include <algorithm>
#include <functional>

namespace Air
{
	// 常量定义
	/////////////////////////////////////////////////////////////////////////////////
	float const PI = 3.141592f;			// PI
	float const PI2 = 6.283185f;			// PI * 2
	float const PIdiv2 = 1.570796f;			// PI / 2

	float const DEG90 = 1.570796f;			// 90 度
	float const DEG270 = -1.570796f;			// 270 度
	float const DEG45 = 0.7853981f;			// 45 度
	float const DEG5 = 0.0872664f;			// 5 度
	float const DEG10 = 0.1745329f;			// 10 度
	float const DEG20 = 0.3490658f;			// 20 度
	float const DEG30 = 0.5235987f;			// 30 度
	float const DEG60 = 1.047197f;			// 60 度
	float const DEG120 = 2.094395f;			// 120 度

	float const DEG40 = 0.6981317f;			// 40 度
	float const DEG80 = 1.396263f;			// 80 度
	float const DEG140 = 2.443460f;			// 140 度
	float const DEG160 = 2.792526f;			// 160 度

	float const SQRT2 = 1.414213f;			// 根2
	float const SQRT_2 = 0.7071068f;			// 1 / SQRT2
	float const SQRT3 = 1.732050f;			// 根3

	float const DEG2RAD = 0.01745329f;			// 角度化弧度因数
	float const RAD2DEG = 57.29577f;			// 弧度化角度因数


	enum BoundOverlap
	{
		BO_No,
		BO_Partial,
		BO_Yes
	};

	namespace MathLib
	{
		// 求绝对值
		template <typename T>
		inline T
			abs(T const & x) AIR_NOEXCEPT
		{
			return x < T(0) ? -x : x;
		}
		// 角度化弧度
		template <typename T>
		inline T
			deg2rad(T const & x) AIR_NOEXCEPT
		{
			return static_cast<T>(x * DEG2RAD);
		}
		// 弧度化角度
		template <typename T>
		inline T
			rad2deg(T const & x) AIR_NOEXCEPT
		{
			return static_cast<T>(x * RAD2DEG);
		}

		// 取小于等于x的最大整数
		template <typename T>
		inline T
			floor(T const & x) AIR_NOEXCEPT
		{
			return static_cast<T>(static_cast<int>(x > 0 ? x : (x - 1)));
		}

		// 取x的小数部分
		template <typename T>
		inline T
			frac(T const & x) AIR_NOEXCEPT
		{
			return x - static_cast<int>(x);
		}

		// 四舍五入
		template <typename T>
		inline T
			round(T const & x) AIR_NOEXCEPT
		{
			return (x > 0) ? static_cast<T>(static_cast<int>(T(0.5) + x)) :
				-static_cast<T>(static_cast<int>(T(0.5) - x));
		}
		// 取整
		template <typename T>
		inline T
			trunc(T const & x) AIR_NOEXCEPT
		{
			return static_cast<T>(static_cast<int>(x));
		}

		// 取三个中小的
		template <typename T>
		inline T const &
			min3(T const & a, T const & b, T const & c) AIR_NOEXCEPT
		{
			return std::min(std::min(a, b), c);
		}
		// 取三个中大的
		template <typename T>
		inline T const &
			max3(T const & a, T const & b, T const & c) AIR_NOEXCEPT
		{
			return std::max(std::max(a, b), c);
		}

		// 余数
		template <typename T>
		inline T
			mod(T const & x, T const & y) AIR_NOEXCEPT
		{
			return x % y;
		}
		// 浮点版本
		template<>
		inline float
			mod<float>(float const & x, float const & y) AIR_NOEXCEPT
		{
			return std::fmod(x, y);
		}
		template <>
		inline double
			mod<double>(double const & x, double const & y) AIR_NOEXCEPT
		{
			return std::fmod(x, y);
		}

		// 限制 val 在 low 和 high 之间
		template <typename T>
		inline T const &
			clamp(T const & val, T const & low, T const & high) AIR_NOEXCEPT
		{
			return std::max(low, std::min(high, val));
		}

		// 环绕处理
		template <typename T>
		inline T
			wrap(T const & val, T const & low, T const & high) AIR_NOEXCEPT
		{
			T range = high - low;
			return val - floor(val / range) * range;
		}

		// 镜像处理
		template <typename T>
		inline T
			mirror(T const & val, T const & low, T const & high) AIR_NOEXCEPT
		{
			T range = high - low;
			int selection_coord = static_cast<int>(floor(val / range));
			return (selection_coord & 1 ? (1 + selection_coord) * range - val : val - selection_coord * range);
		}

		// 奇数则返回true
		template <typename T>
		inline bool
			is_odd(T const & x) AIR_NOEXCEPT
		{
			return mod(x, 2) != 0;
		}
		// 偶数则返回true
		template <typename T>
		inline bool
			is_even(T const & x) AIR_NOEXCEPT
		{
			return !is_odd(x);
		}

		// 判断 val 是否在 low 和 high 之间
		template <typename T>
		inline bool
			in_bound(T const & val, T const & low, T const & high) AIR_NOEXCEPT
		{
			return ((val >= low) && (val <= high));
		}

		// 判断两个数是否相等
		template <typename T>
		inline bool
			equal(T const & lhs, T const & rhs) AIR_NOEXCEPT
		{
			return (lhs == rhs);
		}
		// 浮点版本
		template <>
		inline bool
			equal<float>(float const & lhs, float const & rhs) AIR_NOEXCEPT
		{
			return (abs<float>(lhs - rhs)
				<= std::numeric_limits<float>::epsilon());
		}
		template <>
		inline bool
			equal<double>(double const & lhs, double const & rhs) AIR_NOEXCEPT
		{
			return (abs<double>(lhs - rhs)
				<= std::numeric_limits<double>::epsilon());
		}


		// 基本数学运算
		///////////////////////////////////////////////////////////////////////////////
		float abs(float x) AIR_NOEXCEPT;
		float sqrt(float x) AIR_NOEXCEPT;
		float recip_sqrt(float number) AIR_NOEXCEPT;

		float pow(float x, float y) AIR_NOEXCEPT;
		float exp(float x) AIR_NOEXCEPT;

		float log(float x) AIR_NOEXCEPT;
		float log10(float x) AIR_NOEXCEPT;

		float sin(float x) AIR_NOEXCEPT;
		float cos(float x) AIR_NOEXCEPT;
		void sincos(float x, float& s, float& c) AIR_NOEXCEPT;
		float tan(float x) AIR_NOEXCEPT;

		float asin(float x) AIR_NOEXCEPT;
		float acos(float x) AIR_NOEXCEPT;
		float atan(float x) AIR_NOEXCEPT;

		float sinh(float x) AIR_NOEXCEPT;
		float cosh(float x) AIR_NOEXCEPT;
		float tanh(float x) AIR_NOEXCEPT;

		template <typename T>
		typename T::value_type dot(T const & lhs, T const & rhs) AIR_NOEXCEPT;

		template <typename T>
		Vector_T<T, 3> cross(Vector_T<T, 3> const & lhs, Vector_T<T, 3> const & rhs) AIR_NOEXCEPT;

		template<typename T>
		typename T::value_type length_sq(T const & rhs) AIR_NOEXCEPT;

		template <typename T>
		typename T::value_type length(T const & rhs) AIR_NOEXCEPT;

		template<typename T>
		T transform_coord(T const & v, Matrix4_T<typename T::value_type> const & mat) AIR_NOEXCEPT;

		template<typename T>
		T normalize(T const & rhs) AIR_NOEXCEPT;

		template<typename T>
		Plane_T<T> normalize(Plane_T<T> const & rhs) AIR_NOEXCEPT;

		template <typename T>
		T dot_coord(Plane_T<T> const & lhs, Vector_T<T, 3> const & rhs) AIR_NOEXCEPT;

		template<typename T>
		bool intersect_point_frustum(Vector_T<T, 3> const & v, Frustum_T<T> const & frustum) AIR_NOEXCEPT;

		template<typename T>
		bool intersect_aabb_aabb(AABBox_T<T> const & lhs, AABBox_T<T> const & aabb) AIR_NOEXCEPT;

		template <typename T>
		BoundOverlap intersect_aabb_frustum(AABBox_T<T> const & lhs, Frustum_T<T> const & frustum) AIR_NOEXCEPT;

		template <typename T>
		AABBox_T<T> transform_aabb(AABBox_T<T> const & aabb, Matrix4_T<T> const & matrix) AIR_NOEXCEPT;

		template <typename T>
		AABBox_T<T> transform_aabb(AABBox_T<T> const & aabb, Vector_T<T, 3> const & scale, Quaternion_T<T> const & rot, Vector_T<T, 3> const & trans) AIR_NOEXCEPT;

		template <typename T>
		Vector_T<T, 3> transform_quat(Vector_T<T, 3> const & v, Quaternion_T<T> const & quat) AIR_NOEXCEPT;

		template <typename T>
		T minimize(T const & lhs, T const & rhs) AIR_NOEXCEPT;

		template <typename T>
		T maximize(T const & lhs, T const & rhs) AIR_NOEXCEPT;

		template <typename T>
		BoundOverlap intersect_frustum_frustum(Frustum_T<T> const & lhs, Frustum_T<T> const & frustum) AIR_NOEXCEPT;

		template <typename T>
		void decompose(Vector_T<T, 3> & scale, Quaternion_T<T> & rot, Vector_T<T, 3>& trans, Matrix4_T<T> const & rhs) AIR_NOEXCEPT;

		template <typename T>
		Quaternion_T<T> to_quaternion(Matrix4_T<T> const & mat) AIR_NOEXCEPT;

		template<typename T>
		Color_T<T>	negative(Color_T<T> const & rhs) AIR_NOEXCEPT;

		template <typename T>
		T determinant(Matrix4_T<T> const & rhs) AIR_NOEXCEPT;

		template<typename T>
		Color_T<T> modulate(Color_T<T> const & lhs, Color_T<T> const & rhs) AIR_NOEXCEPT;

		template<typename T>
		Matrix4_T<T> scaling(T const & sx, T const & sy, T const & sz) AIR_NOEXCEPT;


		template<typename T>
		Matrix4_T<T> scaling(Vector_T<T, 3> const & s) AIR_NOEXCEPT;

		template<typename T>
		Matrix4_T<T> translation(T const & tx, T const & ty, T const & tz) AIR_NOEXCEPT;


		template<typename T>
		Matrix4_T<T> translation(Vector_T<T, 3> const & t) AIR_NOEXCEPT;

		template<typename T>
		Quaternion_T<T> mul(Quaternion_T<T> const & lhs, Quaternion_T<T> const & rhs) AIR_NOEXCEPT;

		template <typename T>
		Matrix4_T<T> mul(Matrix4_T<T> const & lhs, Matrix4_T<T> const & rhs) AIR_NOEXCEPT;

		template <typename T>
		Matrix4_T<T> inverse(Matrix4_T<T> const & rhs) AIR_NOEXCEPT;

		template<typename T>
		Matrix4_T<T> look_at_lh(Vector_T<T, 3> const &vEye, Vector_T<T, 3> const & vAt) AIR_NOEXCEPT;

		template<typename T>
		Matrix4_T<T> look_at_lh(Vector_T<T, 3> const & vEye, Vector_T<T, 3> const & vAt, Vector_T<T, 3> const &vUp) AIR_NOEXCEPT;

		template <typename T>
		Matrix4_T<T> perspective_fov_lh(T const & fov, T const & aspect, T const & nearPlane, T const & farPlane) AIR_NOEXCEPT;

		template <typename T>
		Matrix4_T<T> ortho_lh(T const & w, T const & h, T const & nearPlane, T const & farPlane) AIR_NOEXCEPT;

		template <typename T>
		Matrix4_T<T> ortho_off_center_lh(T const & left, T const & right, T const & bottom, T const & top,
			T const & nearPlane, T const & farPlane) AIR_NOEXCEPT;

		template<typename T>
		Matrix4_T<T> transpose(Matrix4_T<T> const & lhs) AIR_NOEXCEPT;

		template <typename T>
		T perspective_area(Vector_T<T, 3> const & view_pos, Matrix4_T<T> const & view_proj, AABBox_T<T> const & aabbox) AIR_NOEXCEPT;
	}
}

#include "basic/include/Vector.hpp"
#include "basic/include/Bound.hpp"
#include "basic/include/Plane.hpp"
#include "basic/include/AABBBox.hpp"
#include "basic/include/Frustum.hpp"
#include "basic/include/Matrix.hpp"
#include "basic/include/Quaternion.hpp"



#endif