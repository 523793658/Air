#ifndef _Basic_Math_Helper_H_
#define _Basic_Math_Helper_H_
#pragma once

namespace Air
{
	template<typename T, int N>
	class Vector_T;
	template<typename T>
	class Matrix4_T;

	namespace detail
	{
		template <typename T, int N>
		struct dot_helper
		{
			static T doDot(T const * lhs, T const * rhs) AIR_NOEXCEPT
			{
				return lhs[0] * rhs[0] + dot_helper<T, N - 1>::doDot(lhs + 1, rhs + 1);
			}
		};
		template <typename T>
		struct dot_helper<T, 1>
		{
			static T doDot(T const * lhs, T const *rhs) AIR_NOEXCEPT
			{
				return lhs[0] * rhs[0];
			}
		};

		template<typename T, int N>
		struct max_minimize_helper
		{
			static void doMax(T out[N], T const lhs[N], T const rhs[N]) AIR_NOEXCEPT
			{
				out[0] = std::max<T>(lhs[0], rhs[0]);
				max_minimize_helper<T, N - 1>::doMax(out + 1, lhs + 1, rhs + 1);
			}

			static void doMin(T out[N], T const lhs[N], T const rhs[N]) AIR_NOEXCEPT
			{
				out[0] = std::min<T>(lhs[0], rhs[0]);
				max_minimize_helper<T, N - 1>::doMin(out + 1, lhs + 1, rhs + 1);
			}
		};

		template<typename T>
		struct max_minimize_helper<T, 1>
		{
			static void doMax(T out[1], T const lhs[1], T const rhs[1]) AIR_NOEXCEPT
			{
				out[0] = std::max<T>(lhs[0], rhs[0]);
			}
			static void doMin(T out[1], T const lhs[1], T const rhs[1]) AIR_NOEXCEPT
			{
				out[0] = std::max<T>(lhs[0], rhs[0]);
			}
		};


		template <typename T, int N>
		struct transform_helper
		{
			static Vector_T<T, 4> doTransform(Vector_T<T, N> const & v, Matrix4_T<T> const & mat) AIR_NOEXCEPT;
		};
		template <typename T>
		struct transform_helper<T, 4>
		{
			static Vector_T<T, 4> doTransform(Vector_T<T, 4> const & v, Matrix4_T<T> const & mat) AIR_NOEXCEPT
			{
				return Vector_T<T, 4>(v.x() * mat(0, 0) + v.y() * mat(1, 0) + v.z() * mat(2, 0) + v.w() * mat(3, 0),
					v.x() * mat(0, 1) + v.y() * mat(1, 1) + v.z() * mat(2, 1) + v.w() * mat(3, 1),
					v.x() * mat(0, 2) + v.y() * mat(1, 2) + v.z() * mat(2, 2) + v.w() * mat(3, 2),
					v.x() * mat(0, 3) + v.y() * mat(1, 3) + v.z() * mat(2, 3) + v.w() * mat(3, 3));
			}
		};
		template <typename T>
		struct transform_helper<T, 3>
		{
			static Vector_T<T, 4> doTransform(Vector_T<T, 3> const & v, Matrix4_T<T> const & mat) AIR_NOEXCEPT
			{
				return Vector_T<T, 4>(v.x() * mat(0, 0) + v.y() * mat(1, 0) + v.z() * mat(2, 0) + mat(3, 0),
					v.x() * mat(0, 1) + v.y() * mat(1, 1) + v.z() * mat(2, 1) + mat(3, 1),
					v.x() * mat(0, 2) + v.y() * mat(1, 2) + v.z() * mat(2, 2) + mat(3, 2),
					v.x() * mat(0, 3) + v.y() * mat(1, 3) + v.z() * mat(2, 3) + mat(3, 3));
			}
		};
		template <typename T>
		struct transform_helper<T, 2>
		{
			static Vector_T<T, 4> doTransform(Vector_T<T, 2> const & v, Matrix4_T<T> const & mat) AIR_NOEXCEPT
			{
				return Vector_T<T, 4>(v.x() * mat(0, 0) + v.y() * mat(1, 0) + mat(3, 0),
					v.x() * mat(0, 1) + v.y() * mat(1, 1) + mat(3, 1),
					v.x() * mat(0, 2) + v.y() * mat(1, 2) + mat(3, 2),
					v.x() * mat(0, 3) + v.y() * mat(1, 3) + mat(3, 3));
			}
		};

		template <typename T, int N>
		struct transform_normal_helper
		{
			static Vector_T<T, N> doTransformNormal(Vector_T<T, N> const & v, Matrix4_T<T> const & mat) AIR_NOEXCEPT;
		};
		template <typename T>
		struct transform_normal_helper<T, 3>
		{
			static Vector_T<T, 3> doTransformNormal(Vector_T<T, 3> const & v, Matrix4_T<T> const & mat) AIR_NOEXCEPT
			{
				Vector_T<T, 4> temp(v.x(), v.y(), v.z(), T(0));
				temp = transform_helper<T, 4>::doTransformNormal(temp, mat);
				return Vector_T<T, 3>(temp.x(), temp.y(), temp.z());
			}
		};
		template <typename T>
		struct transform_normal_helper<T, 2>
		{
			static Vector_T<T, 2> doTransformNormal(Vector_T<T, 2> const & v, Matrix4_T<T> const & mat) AIR_NOEXCEPT
			{
				Vector_T<T, 3> temp(v.x(), v.y(), T(0));
				temp = transform_normal_helper<T, 3>::doTransformNormal(temp, mat);
				return Vector_T<T, 2>(temp.x(), temp.y());
			}
		};

		template <typename T, int N>
		struct vector_helper
		{
			template <typename U>
			static void doCopy(T out[N], U const rhs[N]) AIR_NOEXCEPT
			{
				out[0] = static_cast<T>(rhs[0]);
				vector_helper<T, N - 1>::doCopy(out + 1, rhs + 1);
			}

			static void doAssign(T out[N], T const & rhs) AIR_NOEXCEPT
			{
				out[0] = rhs;
				vector_helper<T, N - 1>::doAssign(out + 1, rhs);
			}

			static void doAdd(T out[N], T const lhs[N], T const rhs[N]) AIR_NOEXCEPT
			{
				out[0] = lhs[0] + rhs[0];
				vector_helper<T, N - 1>::doAdd(out + 1, lhs + 1, rhs + 1);
			}

			static void doAdd(T out[N], T const lhs[N], T const & rhs) AIR_NOEXCEPT
			{
				out[0] = lhs[0] + rhs;
				vector_helper<T, N - 1>::doAdd(out + 1, lhs + 1, rhs);
			}

			static void doSub(T out[N], T const lhs[N], T const rhs[N]) AIR_NOEXCEPT
			{
				out[0] = lhs[0] - rhs[0];
				vector_helper<T, N - 1>::doSub(out + 1, lhs + 1, rhs + 1);
			}

			static void doSub(T out[N], T const lhs[N], T const & rhs) AIR_NOEXCEPT
			{
				out[0] = lhs[0] - rhs;
				vector_helper<T, N - 1>::doSub(out + 1, lhs + 1, rhs);
			}

			static void doMul(T out[N], T const lhs[N], T const rhs[N]) AIR_NOEXCEPT
			{
				out[0] = lhs[0] * rhs[0];
				vector_helper<T, N - 1>::doMul(out + 1, lhs + 1, rhs + 1);
			}

			static void doScale(T out[N], T const lhs[N], T const & rhs) AIR_NOEXCEPT
			{
				out[0] = lhs[0] * rhs;
				vector_helper<T, N - 1>::doScale(out + 1, lhs + 1, rhs);
			}

			static void doDiv(T out[N], T const lhs[N], T const rhs[N]) AIR_NOEXCEPT
			{
				out[0] = lhs[0] / rhs[0];
				vector_helper<T, N - 1>::doDiv(out + 1, lhs + 1, rhs + 1);
			}

			static void doNegate(T out[N], T const rhs[N]) AIR_NOEXCEPT
			{
				out[0] = -rhs[0];
				vector_helper<T, N - 1>::doNegate(out + 1, rhs + 1);
			}

			static bool doEqual(T const lhs[N], T const rhs[N]) AIR_NOEXCEPT
			{
				return vector_helper<T, 1>::doEqual(lhs, rhs) && vector_helper<T, N - 1>::DoEqual(lhs + 1, rhs + 1);
			}

			static void doSwap(T lhs[N], T rhs[N]) AIR_NOEXCEPT
			{
				std::swap(lhs[0], rhs[0]);
				return vector_helper<T, N - 1>::doSwap(lhs + 1, rhs + 1);
			}
		};

		template <typename T>
		struct vector_helper<T, 1>
		{
			template <typename U>
			static void doCopy(T out[1], U const rhs[1]) AIR_NOEXCEPT
			{
				out[0] = static_cast<T>(rhs[0]);
			}

			static void doAssign(T out[1], T const & rhs) AIR_NOEXCEPT
			{
				out[0] = rhs;
			}

			static void doAdd(T out[1], T const lhs[1], T const rhs[1]) AIR_NOEXCEPT
			{
				out[0] = lhs[0] + rhs[0];
			}

			static void doAdd(T out[1], T const lhs[1], T const rhs) AIR_NOEXCEPT
			{
				out[0] = lhs[0] + rhs;
			}

			static void doSub(T out[1], T const lhs[1], T const rhs[1]) AIR_NOEXCEPT
			{
				out[0] = lhs[0] - rhs[0];
			}

			static void doSub(T out[1], T const lhs[1], T const & rhs) AIR_NOEXCEPT
			{
				out[0] = lhs[0] - rhs;
			}

			static void doMul(T out[1], T const lhs[1], T const rhs[1]) AIR_NOEXCEPT
			{
				out[0] = lhs[0] * rhs[0];
			}

			static void doScale(T out[1], T const lhs[1], T const & rhs) AIR_NOEXCEPT
			{
				out[0] = lhs[0] * rhs;
			}

			static void doDiv(T out[1], T const lhs[1], T const rhs[1]) AIR_NOEXCEPT
			{
				out[0] = lhs[0] / rhs[0];
			}

			static void doNegate(T out[1], T const rhs[1]) AIR_NOEXCEPT
			{
				out[0] = -rhs[0];
			}

			static bool doEqual(T const lhs[1], T const rhs[1]) AIR_NOEXCEPT
			{
				return lhs[0] == rhs[0];
			}

			static void doSwap(T lhs[1], T rhs[1]) AIR_NOEXCEPT
			{
				std::swap(lhs[0], rhs[0]);
			}
		};
	}
}

#endif