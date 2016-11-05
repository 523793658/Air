#ifndef _Basic_Math_H_
#define _Basic_Math_H_
#pragma once

#include "DirectXmath.h"

using namespace DirectX;

typedef XMFLOAT3 Vector3;
typedef XMFLOAT2 Vector2;


namespace MathLib
{
	// 求绝对值
	template <typename T>
	inline T
		abs(T const & x) AIR_NOEXCEPT
	{
		return x < T(0) ? -x : x;
	}
	template <typename T, int N>
	Vector_T<T, N> abs(Vector_T<T, N> const & x) AIR_NOEXCEPT;

	// 取符号
	template <typename T>
	inline T
		sgn(T const & x) AIR_NOEXCEPT
	{
		return x < T(0) ? T(-1) : (x > T(0) ? T(1) : T(0));
	}
	template <typename T, int N>
	Vector_T<T, N> sgn(Vector_T<T, N> const & x) AIR_NOEXCEPT;

	// 平方
	template <typename T>
	inline T
		sqr(T const & x) AIR_NOEXCEPT
	{
		return x * x;
	}
	template <typename T, int N>
	Vector_T<T, N> sqr(Vector_T<T, N> const & x) AIR_NOEXCEPT;
	// 立方
	template <typename T>
	inline T
		cube(T const & x) AIR_NOEXCEPT
	{
		return sqr(x) * x;
	}
	template <typename T, int N>
	Vector_T<T, N> cube(Vector_T<T, N> const & x) AIR_NOEXCEPT;

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

}



#endif