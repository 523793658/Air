#ifndef _Basic_Math_H_
#define _Basic_Math_H_
#pragma once

namespace Air
{
#ifdef FLAMEMATH
#include "FlameMath.h"
#endif
	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	float const PI = 3.141592f;			// PI
	float const PI2 = 6.283185f;			// PI * 2
	float const PIdiv2 = 1.570796f;			// PI / 2

	float const DEG90 = 1.570796f;			// 90 ��
	float const DEG270 = -1.570796f;			// 270 ��
	float const DEG45 = 0.7853981f;			// 45 ��
	float const DEG5 = 0.0872664f;			// 5 ��
	float const DEG10 = 0.1745329f;			// 10 ��
	float const DEG20 = 0.3490658f;			// 20 ��
	float const DEG30 = 0.5235987f;			// 30 ��
	float const DEG60 = 1.047197f;			// 60 ��
	float const DEG120 = 2.094395f;			// 120 ��

	float const DEG40 = 0.6981317f;			// 40 ��
	float const DEG80 = 1.396263f;			// 80 ��
	float const DEG140 = 2.443460f;			// 140 ��
	float const DEG160 = 2.792526f;			// 160 ��

	float const SQRT2 = 1.414213f;			// ��2
	float const SQRT_2 = 0.7071068f;			// 1 / SQRT2
	float const SQRT3 = 1.732050f;			// ��3

	float const DEG2RAD = 0.01745329f;			// �ǶȻ���������
	float const RAD2DEG = 57.29577f;			// ���Ȼ��Ƕ�����


	namespace MathLib
	{
		// �����ֵ
		template <typename T>
		inline T
			abs(T const & x) AIR_NOEXCEPT
		{
			return x < T(0) ? -x : x;
		}
		// �ǶȻ�����
		template <typename T>
		inline T
			deg2rad(T const & x) AIR_NOEXCEPT
		{
			return static_cast<T>(x * DEG2RAD);
		}
		// ���Ȼ��Ƕ�
		template <typename T>
		inline T
			rad2deg(T const & x) AIR_NOEXCEPT
		{
			return static_cast<T>(x * RAD2DEG);
		}

		// ȡС�ڵ���x���������
		template <typename T>
		inline T
			floor(T const & x) AIR_NOEXCEPT
		{
			return static_cast<T>(static_cast<int>(x > 0 ? x : (x - 1)));
		}

		// ȡx��С������
		template <typename T>
		inline T
			frac(T const & x) AIR_NOEXCEPT
		{
			return x - static_cast<int>(x);
		}

		// ��������
		template <typename T>
		inline T
			round(T const & x) AIR_NOEXCEPT
		{
			return (x > 0) ? static_cast<T>(static_cast<int>(T(0.5) + x)) :
				-static_cast<T>(static_cast<int>(T(0.5) - x));
		}
		// ȡ��
		template <typename T>
		inline T
			trunc(T const & x) AIR_NOEXCEPT
		{
			return static_cast<T>(static_cast<int>(x));
		}

		// ȡ������С��
		template <typename T>
		inline T const &
			min3(T const & a, T const & b, T const & c) AIR_NOEXCEPT
		{
			return std::min(std::min(a, b), c);
		}
		// ȡ�����д��
		template <typename T>
		inline T const &
			max3(T const & a, T const & b, T const & c) AIR_NOEXCEPT
		{
			return std::max(std::max(a, b), c);
		}

		// ����
		template <typename T>
		inline T
			mod(T const & x, T const & y) AIR_NOEXCEPT
		{
			return x % y;
		}
		// ����汾
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

		// ���� val �� low �� high ֮��
		template <typename T>
		inline T const &
			clamp(T const & val, T const & low, T const & high) AIR_NOEXCEPT
		{
			return std::max(low, std::min(high, val));
		}

		// ���ƴ���
		template <typename T>
		inline T
			wrap(T const & val, T const & low, T const & high) AIR_NOEXCEPT
		{
			T range = high - low;
			return val - floor(val / range) * range;
		}

		// ������
		template <typename T>
		inline T
			mirror(T const & val, T const & low, T const & high) AIR_NOEXCEPT
		{
			T range = high - low;
			int selection_coord = static_cast<int>(floor(val / range));
			return (selection_coord & 1 ? (1 + selection_coord) * range - val : val - selection_coord * range);
		}

		// �����򷵻�true
		template <typename T>
		inline bool
			is_odd(T const & x) AIR_NOEXCEPT
		{
			return mod(x, 2) != 0;
		}
		// ż���򷵻�true
		template <typename T>
		inline bool
			is_even(T const & x) AIR_NOEXCEPT
		{
			return !is_odd(x);
		}

		// �ж� val �Ƿ��� low �� high ֮��
		template <typename T>
		inline bool
			in_bound(T const & val, T const & low, T const & high) AIR_NOEXCEPT
		{
			return ((val >= low) && (val <= high));
		}

		// �ж��������Ƿ����
		template <typename T>
		inline bool
			equal(T const & lhs, T const & rhs) AIR_NOEXCEPT
		{
			return (lhs == rhs);
		}
		// ����汾
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


		// ������ѧ����
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
}
#endif