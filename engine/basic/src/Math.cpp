#include "basic/include/Basic.h"
#include "basic/include/Frustum.hpp"
#include "basic/include/Plane.hpp"
#include "basic/include/MathHelper.hpp"
#include "basic/include/Matrix.hpp"
#include "basic/include/Quaternion.hpp"
#include "basic/include/Math.hpp"

namespace Air
{
	namespace MathLib
	{
		float abs(float x) AIR_NOEXCEPT
		{
			union FNI
			{
				float f;
				int32_t i;
			} fni;
			fni.f = x;
			fni.i &= 0x7FFFFFFF;
			return fni.f;
		}

		float sqrt(float x) AIR_NOEXCEPT
		{
			return std::sqrt(x);
		}

		// From Quake III. But the magic number is from http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf
		float recip_sqrt(float number) AIR_NOEXCEPT
		{
			float const threehalfs = 1.5f;

			float x2 = number * 0.5f;
			union FNI
			{
				float f;
				int32_t i;
			} fni;
			fni.f = number;											// evil floating point bit level hacking
			fni.i = 0x5f375a86 - (fni.i >> 1);						// what the fuck?
			fni.f = fni.f * (threehalfs - (x2 * fni.f * fni.f));	// 1st iteration
			fni.f = fni.f * (threehalfs - (x2 * fni.f * fni.f));		// 2nd iteration, this can be removed

			return fni.f;
		}

		float pow(float x, float y) AIR_NOEXCEPT
		{
			return std::pow(x, y);
		}

		float exp(float x) AIR_NOEXCEPT
		{
			return std::exp(x);
		}

		float log(float x) AIR_NOEXCEPT
		{
			return std::log(x);
		}

		float log10(float x) AIR_NOEXCEPT
		{
			return std::log10(x);
		}

		float sin(float x) AIR_NOEXCEPT
		{
			return std::sin(x);
		}

		float cos(float x) AIR_NOEXCEPT
		{
			return sin(x + PI / 2);
		}

		void sincos(float x, float& s, float& c) AIR_NOEXCEPT
		{
			s = sin(x);
			c = cos(x);
		}

		float tan(float x) AIR_NOEXCEPT
		{
			return std::tan(x);
		}

		float asin(float x) AIR_NOEXCEPT
		{
			return std::asin(x);
		}

		float acos(float x) AIR_NOEXCEPT
		{
			return std::acos(x);
		}

		float atan(float x) AIR_NOEXCEPT
		{
			return std::atan(x);
		}

		float sinh(float x) AIR_NOEXCEPT
		{
			return std::sinh(x);
		}

		float cosh(float x) AIR_NOEXCEPT
		{
			return std::cosh(x);
		}

		float tanh(float x) AIR_NOEXCEPT
		{
			return std::tanh(x);
		}

		template float4x4 transpose(float4x4 const & rhs) AIR_NOEXCEPT;

		template<typename T>
		Matrix4_T<T> transpose(Matrix4_T<T> const & rhs) AIR_NOEXCEPT
		{
			return Matrix4_T<T>(
				rhs(0, 0), rhs(1, 0), rhs(2, 0), rhs(3, 0),
				rhs(0, 1), rhs(1, 1), rhs(2, 1), rhs(3, 1),
				rhs(0, 2), rhs(1, 2), rhs(2, 2), rhs(3, 2),
				rhs(0, 3), rhs(1, 3), rhs(2, 3), rhs(3, 3));
		}

		template
		Quaternion mul(Quaternion const & lhs, Quaternion const & rhs) AIR_NOEXCEPT;

		template<typename T>
		Quaternion_T<T> mul(Quaternion_T<T> const & lhs, Quaternion_T<T> const & rhs) AIR_NOEXCEPT
		{
			return Quaternion_T<T>(
				lhs.x() * rhs.w() - lhs.y() * rhs.z() + lhs.z() * rhs.y() + lhs.w() * rhs.x(),
				lhs.x() * rhs.z() + lhs.y() * rhs.w() - lhs.z() * rhs.x() + lhs.w() * rhs.y(),
				lhs.y() * rhs.x() - lhs.x() * rhs.y() + lhs.z() * rhs.w() + lhs.w() * rhs.z(),
				lhs.w() * rhs.w() - lhs.x() * rhs.x() - lhs.y() * rhs.y() - lhs.z() * rhs.z());
		}

		template float4x4 mul(float4x4 const & lhs, float4x4 const & rhs) AIR_NOEXCEPT;
		
		template <typename T>
		Matrix4_T<T> mul(Matrix4_T<T> const & lhs, Matrix4_T<T> const &rhs) AIR_NOEXCEPT
		{
			return Matrix4_T<T>(
				lhs(0, 0) * rhs(0, 0) + lhs(0, 1) * rhs(1, 0) + lhs(0, 2) * rhs(2, 0) + lhs(0, 3)*rhs(3, 0),
				lhs(0, 0) * rhs(0, 1) + lhs(0, 1) * rhs(1, 1) + lhs(0, 2) * rhs(2, 1) + lhs(0, 3)*rhs(3, 1),
				lhs(0, 0) * rhs(0, 2) + lhs(0, 1) * rhs(1, 2) + lhs(0, 2) * rhs(2, 2) + lhs(0, 3)*rhs(3, 2),
				lhs(0, 0) * rhs(0, 3) + lhs(0, 1) * rhs(1, 3) + lhs(0, 2) * rhs(2, 3) + lhs(0, 3)*rhs(3, 3),

				lhs(1, 0) * rhs(0, 0) + lhs(1, 1) * rhs(1, 0) + lhs(1, 2) * rhs(2, 0) + lhs(1, 3)*rhs(3, 0),
				lhs(1, 0) * rhs(0, 1) + lhs(1, 1) * rhs(1, 1) + lhs(1, 2) * rhs(2, 1) + lhs(1, 3)*rhs(3, 1),
				lhs(1, 0) * rhs(0, 2) + lhs(1, 1) * rhs(1, 2) + lhs(1, 2) * rhs(2, 2) + lhs(1, 3)*rhs(3, 2),
				lhs(1, 0) * rhs(0, 3) + lhs(1, 1) * rhs(1, 3) + lhs(1, 2) * rhs(2, 3) + lhs(1, 3)*rhs(3, 3),

				lhs(2, 0) * rhs(0, 0) + lhs(2, 1) * rhs(1, 0) + lhs(2, 2) * rhs(2, 0) + lhs(2, 3)*rhs(3, 0),
				lhs(2, 0) * rhs(0, 1) + lhs(2, 1) * rhs(1, 1) + lhs(2, 2) * rhs(2, 1) + lhs(2, 3)*rhs(3, 1),
				lhs(2, 0) * rhs(0, 2) + lhs(2, 1) * rhs(1, 2) + lhs(2, 2) * rhs(2, 2) + lhs(2, 3)*rhs(3, 2),
				lhs(2, 0) * rhs(0, 3) + lhs(2, 1) * rhs(1, 3) + lhs(2, 2) * rhs(2, 3) + lhs(2, 3)*rhs(3, 3),

				lhs(3, 0) * rhs(0, 0) + lhs(3, 1) * rhs(1, 0) + lhs(3, 2) * rhs(2, 0) + lhs(3, 3)*rhs(3, 0),
				lhs(3, 0) * rhs(0, 1) + lhs(3, 1) * rhs(1, 1) + lhs(3, 2) * rhs(2, 1) + lhs(3, 3)*rhs(3, 1),
				lhs(3, 0) * rhs(0, 2) + lhs(3, 1) * rhs(1, 2) + lhs(3, 2) * rhs(2, 2) + lhs(3, 3)*rhs(3, 2),
				lhs(3, 0) * rhs(0, 3) + lhs(3, 1) * rhs(1, 3) + lhs(3, 2) * rhs(2, 3) + lhs(3, 3)*rhs(3, 3)
				);
		}

		template float2 transform_coord(float2 const & v, float4x4 const & mat) AIR_NOEXCEPT;
		template float3 transform_coord(float3 const & v, float4x4 const & mat) AIR_NOEXCEPT;

		template<typename T>
		T transform_coord(T const & v, Matrix4_T<typename T::value_type> const & mat) AIR_NOEXCEPT
		{
			static_assert(T::elem_num < 4, "Must be at most 4D vector");
			Vector_T<typename T::value_type, 4> temp(detail::transform_helper<typename T::value_type, T::elem_num>::doF(v, mat));
			Vector_T<typename T::value_type, T::elem_num> ret(&temp[0]);
			if (equal(temp.w(), typename T::value_type(0)))
			{
				ret = T::zero();
			}
			else
			{
				ret /= temp.w();
			}
			return ret;
		}

		template float1 normalize(float1 const & rhs) AIR_NOEXCEPT;
		template float2 normalize(float2 const & rhs) AIR_NOEXCEPT;
		template float3 normalize(float3 const & rhs) AIR_NOEXCEPT;
		template float4 normalize(float4 const & rhs) AIR_NOEXCEPT;

		template <typename T>
		T normalize(T const & rhs) AIR_NOEXCEPT
		{
			return rhs * recip_sqrt(length_sq(rhs));
		}

		template Plane normalize(Plane const & rhs) AIR_NOEXCEPT;

		template <typename T>
		Plane_T<T> normalize(Plane_T<T> const & rhs) AIR_NOEXCEPT
		{
			T const inv(T(1) / length(rhs.getNormal()));
			return Plane_T<T>(rhs.a() * inv, rhs.b() * inv, rhs.c() * inv, rhs.d() * inv);
		}

		template int32_t dot(int1 const & lhs, int1 const & rhs) AIR_NOEXCEPT;
		template int32_t dot(int2 const & lhs, int2 const & rhs) AIR_NOEXCEPT;
		template int32_t dot(int3 const & lhs, int3 const & rhs) AIR_NOEXCEPT;
		template int32_t dot(int4 const & lhs, int4 const & rhs) AIR_NOEXCEPT;
		template uint32_t dot(uint1 const & lhs, uint1 const & rhs) AIR_NOEXCEPT;
		template uint32_t dot(uint2 const & lhs, uint2 const & rhs) AIR_NOEXCEPT;
		template uint32_t dot(uint3 const & lhs, uint3 const & rhs) AIR_NOEXCEPT;
		template uint32_t dot(uint4 const & lhs, uint4 const & rhs) AIR_NOEXCEPT;
		template float dot(float1 const & lhs, float1 const & rhs) AIR_NOEXCEPT;
		template float dot(float2 const & lhs, float2 const & rhs) AIR_NOEXCEPT;
		template float dot(float3 const & lhs, float3 const & rhs) AIR_NOEXCEPT;
		template float dot(float4 const & lhs, float4 const & rhs) AIR_NOEXCEPT;

		template <typename T>
		typename T::value_type dot(T const & lhs, T const & rhs) AIR_NOEXCEPT
		{
			return detail::dot_helper<typename T::value_type, T::elem_num>::dof(&lhs[0], &rhs[0]);
		}


		template int3 cross(int3 const & lhs, int3 const & rhs) AIR_NOEXCEPT;
		template uint3 cross(uint3 const & lhs, uint3 const & rhs) AIR_NOEXCEPT;
		template float3 cross(float3 const & lhs, float3 const & rhs) AIR_NOEXCEPT;

		template <typename T>
		Vector_T<T, 3> cross(Vector_T<T, 3> const & lhs, Vector_T<T, 3> const & rhs) AIR_NOEXCEPT
		{
			return Vector_T<T, 3>(lhs.y() * rhs.z() - lhs.z() * rhs.y(),
				lhs.z() * rhs.x() - lhs.x() * rhs.z(),
				lhs.x() * rhs.y() - lhs.y() * rhs.x());
		}



		template int32_t length_sq(int1 const & rhs) AIR_NOEXCEPT;
		template int32_t length_sq(int2 const & rhs) AIR_NOEXCEPT;
		template int32_t length_sq(int3 const & rhs) AIR_NOEXCEPT;
		template int32_t length_sq(int4 const & rhs) AIR_NOEXCEPT;

		template uint32_t length_sq(uint1 const & rhs) AIR_NOEXCEPT;
		template uint32_t length_sq(uint2 const & rhs) AIR_NOEXCEPT;
		template uint32_t length_sq(uint3 const & rhs) AIR_NOEXCEPT;
		template uint32_t length_sq(uint4 const & rhs) AIR_NOEXCEPT;

		template float length_sq(float1 const & rhs) AIR_NOEXCEPT;
		template float length_sq(float2 const & rhs) AIR_NOEXCEPT;
		template float length_sq(float3 const & rhs) AIR_NOEXCEPT;
		template float length_sq(float4 const & rhs) AIR_NOEXCEPT;

		template<typename T>
		typename T::value_type length_sq(T const & rhs) AIR_NOEXCEPT
		{
			return dot(rhs, rhs);
		}



		template int32_t length(int1 const & rhs) AIR_NOEXCEPT;
		template int32_t length(int2 const & rhs) AIR_NOEXCEPT;
		template int32_t length(int3 const & rhs) AIR_NOEXCEPT;
		template int32_t length(int4 const & rhs) AIR_NOEXCEPT;

		template uint32_t length(uint1 const & rhs) AIR_NOEXCEPT;
		template uint32_t length(uint2 const & rhs) AIR_NOEXCEPT;
		template uint32_t length(uint3 const & rhs) AIR_NOEXCEPT;
		template uint32_t length(uint4 const & rhs) AIR_NOEXCEPT;

		template float length(float1 const & rhs) AIR_NOEXCEPT;
		template float length(float2 const & rhs) AIR_NOEXCEPT;
		template float length(float3 const & rhs) AIR_NOEXCEPT;
		template float length(float4 const & rhs) AIR_NOEXCEPT;


		template <typename T>
		typename T::value_type length(T const & rhs) AIR_NOEXCEPT
		{
			return static_cast<T::value_type>(sqrt(static_cast<float>(length_sq(rhs))));
		}

		template <typename T>
		T dot_coord(Plane_T<T> const & lhs, Vector_T<T, 3> const & rhs) AIR_NOEXCEPT
		{
			return lhs.a() * rhs.x() + lhs.b() * rhs.y() + lhs.b() * rhs.z() + lhs.d();
		}


		template bool intersect_point_frustum(float3 const & v, Frustum const & frustum) AIR_NOEXCEPT;

		template<typename T>
		bool intersect_point_frustum(Vector_T<T, 3> const & v, Frustum_T<T> const & frustum) AIR_NOEXCEPT
		{
			for (int i = 0; i < 6; ++i)
			{
				if (dot_coord(frustum.getFrustumPlane(i), v) < 0)
				{
					return false;
				}
			}
			return true;
		}
		
		template void decompose(float3 & scale, Quaternion& rotate, float3& trans, float4x4 const & rhs) AIR_NOEXCEPT;
		
		template <typename T>
		void decompose(Vector_T<T, 3> & scale, Quaternion_T<T> & rot, Vector_T<T, 3>& trans, Matrix4_T<T> const & rhs) AIR_NOEXCEPT
		{
			scale.x() = sqrt(rhs(0, 0) * rhs(0, 0) + rhs(0, 1) * rhs(0, 1) + rhs(0, 2) * rhs(0, 2));
			scale.y() = sqrt(rhs(1, 0) * rhs(1, 0) + rhs(1, 1) * rhs(1, 1) + rhs(1, 2) * rhs(1, 2));
			scale.z() = sqrt(rhs(2, 0) * rhs(2, 0) + rhs(2, 1) * rhs(2, 1) + rhs(2, 2) * rhs(2, 2));

			trans = Vector_T<T, 3>(rhs(3, 0), rhs(3, 1), rhs(3, 2));

			Matrix4_T<T> rot_mat;
			rot_mat(0, 0) = rhs(0, 0) / scale.x();
			rot_mat(0, 1) = rhs(0, 1) / scale.x();
			rot_mat(0, 2) = rhs(0, 2) / scale.x();
			rot_mat(0, 3) = 0;
			rot_mat(1, 0) = rhs(1, 0) / scale.y();
			rot_mat(1, 1) = rhs(1, 1) / scale.y();
			rot_mat(1, 2) = rhs(1, 2) / scale.y();
			rot_mat(1, 3) = 0;
			rot_mat(2, 0) = rhs(2, 0) / scale.z();
			rot_mat(2, 1) = rhs(2, 1) / scale.z();
			rot_mat(2, 2) = rhs(2, 2) / scale.z();
			rot_mat(2, 3) = 0;
			rot_mat(3, 0) = 0;
			rot_mat(3, 1) = 0;
			rot_mat(3, 2) = 0;
			rot_mat(3, 3) = 1;
			rot = to_quaternion(rot_mat);
		}


		template float4x4 transformation(float3 const * scaling_center, Quaternion const * scaling_rotation, float3 const * scale, float3 const * rotation_center, Quaternion const * rotation, float3 const * trans) AIR_NOEXCEPT;


		template <typename T>
		Matrix4_T<T> transformation(Vector_T<T, 3> const * scaling_center, Quaternion_T<T> const * scaling_rotation, Vector_T<T, 3> const * scale, Vector_T<T, 3> const * rotation_center, Quaternion_T<T> const * rotation, Vector_T<T, 3> const * trans) AIR_NOEXCEPT
		{
			Vector_T<T, 3> psc, prc, pt;
			if (scaling_center)
			{
				psc = *scaling_center;
			}
			else
			{
				psc = Vector_T<T, 3>(T(0), T(0), T(0));
			}
			if (rotation_center)
			{
				prc = *rotation_center;
			}
			else
			{
				prc = Vector_T<T, 3>(T(0), T(0), T(0));
			}
			if (trans)
			{
				pt = *trans;
			}
			else
			{
				pt = Vector_T<T, 3>(T(0), T(0), T(0));
			}
			Matrix4_T<T> m1, m2, m3, m4, m5, m6, m7;
			m1 = translation(-psc);
			if (scaling_rotation)
			{
				m4 = to_matrix(*scaling_rotation);
				m2 = inverse(m4);
			}
			else
			{
				m2 = m4 = Matrix4_T<T>::identify();
			}
			if (scale)
			{
				m3 = scaling(*scale);
			}
			else
			{
				m3 = Matrix4_T<T>::identify();
			}
			if (rotation)
			{
				m6 = to_matrix(*rotation);
			}
			else
			{
				m6 = Matrix4_T<T>::identify();
			}
			m5 = translation(psc - prc);
			m7 = translation(prc + pt);
			return m1 * m2 * m3 * m4 * m5 * m6 * m7;
		}

		template float4x4 to_matrix(Quaternion const & quat) AIR_NOEXCEPT;

		template<typename T>
		Matrix4_T<T> to_matrix(Quaternion_T<T> const & quat) AIR_NOEXCEPT
		{
			T const x2(quat.x() + quat.x());
			T const y2(quat.y() + quat.y());
			T const z2(quat.z() + quat.z());
			T const xx2(quat.x() * x2), xy2(quat.x() * y2), xz2(quat.x() * z2);
			T const yy2(quat.y() * y2), yz2(quat.y() * z2), zz2(quat.z() * z2);
			T const wx2(quat.w() * x2), wy2(quat.w() * y2), wz2(quat.w() * z2);

			return Matrix4_T<T>(
				1 - yy2 - zz2, xy2 + wz2, xz2 - wy2, 0,
				xy2 - wz2, 1 - xx2 - zz2, yz2 + wx2, 0,
				xz2 + wy2, yz2 - wx2, 1 - xx2 - yy2, 0,
				0, 0, 0, 1
				);

		}

		template Quaternion to_quaternion(float4x4 const & mat) AIR_NOEXCEPT;

		template <typename T>
		Quaternion_T<T> to_quaternion(Matrix4_T<T> const & mat) AIR_NOEXCEPT
		{
			Quaternion_T<T> quat;
			T s;
			T const tr = mat(0, 0) + mat(1, 1) + mat(2, 2) + 1;

			// check the diagonal
			if (tr > 1)
			{
				s = sqrt(tr);
				quat.w() = s * T(0.5);
				s = T(0.5) / s;
				quat.x() = (mat(1, 2) - mat(2, 1)) * s;
				quat.y() = (mat(2, 0) - mat(0, 2)) * s;
				quat.z() = (mat(0, 1) - mat(1, 0)) * s;
			}
			else
			{
				int maxi = 0;
				T maxdiag = mat(0, 0);
				for (int i = 1; i < 3; ++i)
				{
					if (mat(i, i) > maxdiag)
					{
						maxi = i;
						maxdiag = mat(i, i);
					}
				}

				switch (maxi)
				{
				case 0:
					s = sqrt((mat(0, 0) - (mat(1, 1) + mat(2, 2))) + 1);

					quat.x() = s * T(0.5);

					if (!equal<T>(s, 0))
					{
						s = T(0.5) / s;
					}

					quat.w() = (mat(1, 2) - mat(2, 1)) * s;
					quat.y() = (mat(1, 0) + mat(0, 1)) * s;
					quat.z() = (mat(2, 0) + mat(0, 2)) * s;
					break;

				case 1:
					s = sqrt((mat(1, 1) - (mat(2, 2) + mat(0, 0))) + 1);
					quat.y() = s * T(0.5);

					if (!equal<T>(s, 0))
					{
						s = T(0.5) / s;
					}

					quat.w() = (mat(2, 0) - mat(0, 2)) * s;
					quat.z() = (mat(2, 1) + mat(1, 2)) * s;
					quat.x() = (mat(0, 1) + mat(1, 0)) * s;
					break;

				case 2:
				default:
					s = sqrt((mat(2, 2) - (mat(0, 0) + mat(1, 1))) + 1);

					quat.z() = s * T(0.5);

					if (!equal<T>(s, 0))
					{
						s = T(0.5) / s;
					}

					quat.w() = (mat(0, 1) - mat(1, 0)) * s;
					quat.x() = (mat(0, 2) + mat(2, 0)) * s;
					quat.y() = (mat(1, 2) + mat(2, 1)) * s;
					break;
				}
			}

			return normalize(quat);
		}

		template Quaternion rotation_axis(float3 const & v, float const & angle) AIR_NOEXCEPT;

		template<typename T>
		Quaternion_T<T> rotation_axis(Vector_T<T, 3> const & v, T const & angle) AIR_NOEXCEPT
		{
			T sa, ca;
			sincos(angle * T(0.5), sa, ca);
			if (equal<T>(length_sq(v), 0))
			{
				return Quaternion_T<T>(sa, sa, sa, ca);
			}
			else
			{
				return Quaternion_T<T>(sa * normalize(v), ca);
			}
		}

		template int1 minimize(int1 const & lhs, int1 const & rhs) AIR_NOEXCEPT;
		template int2 minimize(int2 const & lhs, int2 const & rhs) AIR_NOEXCEPT;
		template int3 minimize(int3 const & lhs, int3 const & rhs) AIR_NOEXCEPT;
		template int4 minimize(int4 const & lhs, int4 const & rhs) AIR_NOEXCEPT;
		template uint1 minimize(uint1 const & lhs, uint1 const & rhs) AIR_NOEXCEPT;
		template uint2 minimize(uint2 const & lhs, uint2 const & rhs) AIR_NOEXCEPT;
		template uint3 minimize(uint3 const & lhs, uint3 const & rhs) AIR_NOEXCEPT;
		template uint4 minimize(uint4 const & lhs, uint4 const & rhs) AIR_NOEXCEPT;
		template float1 minimize(float1 const & lhs, float1 const & rhs) AIR_NOEXCEPT;
		template float2 minimize(float2 const & lhs, float2 const & rhs) AIR_NOEXCEPT;
		template float3 minimize(float3 const & lhs, float3 const & rhs) AIR_NOEXCEPT;
		template float4 minimize(float4 const & lhs, float4 const & rhs) AIR_NOEXCEPT;

		template <typename T>
		T minimize(T const & lhs, T const & rhs) AIR_NOEXCEPT
		{
			T ret;
			detail::max_miniminze_helper<typename T::value_type,
				T::elem_num>::doMin(&ret[0], &lhs[0], &rhs[0]);
			return ret;
		}


		template int1 maximize(int1 const & lhs, int1 const & rhs) AIR_NOEXCEPT;
		template int2 maximize(int2 const & lhs, int2 const & rhs) AIR_NOEXCEPT;
		template int3 maximize(int3 const & lhs, int3 const & rhs) AIR_NOEXCEPT;
		template int4 maximize(int4 const & lhs, int4 const & rhs) AIR_NOEXCEPT;
		template uint1 maximize(uint1 const & lhs, uint1 const & rhs) AIR_NOEXCEPT;
		template uint2 maximize(uint2 const & lhs, uint2 const & rhs) AIR_NOEXCEPT;
		template uint3 maximize(uint3 const & lhs, uint3 const & rhs) AIR_NOEXCEPT;
		template uint4 maximize(uint4 const & lhs, uint4 const & rhs) AIR_NOEXCEPT;
		template float1 maximize(float1 const & lhs, float1 const & rhs) AIR_NOEXCEPT;
		template float2 maximize(float2 const & lhs, float2 const & rhs) AIR_NOEXCEPT;
		template float3 maximize(float3 const & lhs, float3 const & rhs) AIR_NOEXCEPT;
		template float4 maximize(float4 const & lhs, float4 const & rhs) AIR_NOEXCEPT;
		template <typename T>
		T maximize(T const & lhs, T const & rhs) AIR_NOEXCEPT
		{
			T ret;
			detail::max_miniminze_helper<typename T::value_type, T::elem_num>::doMax(&ret[0], &lhs[0], &rhs[0]);
			return ret;
		}

		template AABBox transform_aabb(AABBox const & aabb, float4x4 const & matrix) AIR_NOEXCEPT;

		template <typename T>
		AABBox_T<T> transform_aabb(AABBox_T<T> const & aabb, Matrix4_T<T> const & matrix) AIR_NOEXCEPT
		{
			Vector_T<T, 3> scale, trans;
			Quaternion_T<T> rot;
			decompose(scale, rot, trans, matrix);
			return transform_aabb(aabb, scale, rot, trans);
		}

		template AABBox transform_aabb(AABBox const & aabb, float3 const & scale, Quaternion const & rot, float3 const & trans) AIR_NOEXCEPT;

		template <typename T>
		AABBox_T<T> transform_aabb(AABBox_T<T> const & aabb, Vector_T<T, 3> const & scale, Quaternion_T<T> const & rot, Vector_T<T, 3> const & trans) AIR_NOEXCEPT
		{
			Vector_T<T, 3> min, max;
			min = max = transform_quat(aabb.getCorner(0) * scale, rot) + trans;
			for (size_t j = 1; j < 8; ++j)
			{
				Vector_T<T, 3> const vec = transform_quat(aabb.getCorner(j) * scale, rot) + trans;
				min = minimize(min, vec);
				max = maximize(max, vec);
			}

			return AABBox_T<T>(min, max);
		}




		template float3 transform_quat(float3 const & v, Quaternion const & quat) AIR_NOEXCEPT;

		template <typename T>
		Vector_T<T, 3> transform_quat(Vector_T<T, 3> const & v, Quaternion_T<T> const & quat) AIR_NOEXCEPT
		{
			return v + cross(quat.v(), cross(quat.v(), v) + quat.w() * v) * T(2);
		}

		template bool intersect_aabb_aabb(AABBox const & lhs, AABBox const & aabb) AIR_NOEXCEPT;

		template<typename T>
		bool intersect_aabb_aabb(AABBox_T<T> const & lhs, AABBox_T<T> const & aabb) AIR_NOEXCEPT
		{
			Vector_T<T, 3> const t = aabb.getCenter() - lhs.getCenter();
			Vector_T<T, 3> const e = aabb.getHalfSize() + lhs.getHalfSize();
			return (MathLib::abs(t.x()) <= e.x()) && (MathLib::abs(t.y()) <= e.y()) && (MathLib::abs(t.z()) <= e.z());
		}

		template BoundOverlap intersect_aabb_frustum(AABBox const & aabb, Frustum const & frustum) AIR_NOEXCEPT;

		template <typename T>
		BoundOverlap intersect_aabb_frustum(AABBox_T<T> const & aabb, Frustum_T<T> const & frustum) AIR_NOEXCEPT
		{
			Vector_T<T, 3> const & min_pt = aabb.getMin();
			Vector_T<T, 3> const & max_pt = aabb.getMax();

			bool intersect = false;
			for (int i = 0; i < 6; ++i)
			{
				Plane_T<T> const & plane = frustum.getFrustumPlane(i);

				Vector_T<T, 3> v0((plane.a() < 0) ? min_pt.x() : max_pt.x(), (plane.b() < 0) ? min_pt.y() : max_pt.y(), (plane.c() < 0) ? min_pt.z() : max_pt.z());

				Vector_T<T, 3> v1((plane.a() < 0) ? max_pt.x() : min_pt.x(), (plane.b() < 0) ? max_pt.y() : min_pt.y(), (plane.c() < 0) ? max_pt.z() : min_pt.z());
				if (dot_coord(plane, v0) < 0)
				{
					return BO_No;
				}
				if (dot_coord(plane, v1) < 0)
				{
					intersect = true;
				}
			}
			return intersect ? BO_Partial : BO_Yes;
		}

		template BoundOverlap intersect_frustum_frustum ( Frustum const & lhs, Frustum const & frustum) AIR_NOEXCEPT;

		template <typename T>
		BoundOverlap intersect_frustum_frustum(Frustum_T<T> const & lhs, Frustum_T<T> const & rhs) AIR_NOEXCEPT
		{
			bool outside = false;
			bool inside_all = true;

			for (int i = 0; i < 6; ++i)
			{
				Plane_T<T> const & p = lhs.getFrustumPlane(i);
				T min_p, max_p;
				min_p = max_p = dot_coord(p, rhs.getCorner(0));
				for (int j = 1; j < 8; ++j)
				{
					T tmp = dot_coord(p, rhs.getCorner(j));
					min_p = std::min(min_p, tmp);
					max_p = std::max(max_p, tmp);
				}
				outside |= (min_p > 0);
				inside_all &= (max_p <= 0);
			}
			if (outside)
			{
				return BO_No;
			}
			if (inside_all)
			{
				return BO_Yes;
			}
			for (int i = 0; i < 6; ++i)
			{
				Plane_T<T> const& p = rhs.getFrustumPlane(i);
				T min_p = dot_coord(p, lhs.getCorner(0));
				for (int j = 1; j < 8; ++j)
				{
					T tmp = dot_coord(p, lhs.getCorner(j));
					min_p = std::min(min_p, tmp);
				}
				outside |= (min_p > 0);
			}
			if (outside)
			{
				return BO_No;
			}
			Vector_T<T, 3> edge_axis_l[6];
			edge_axis_l[0] = rhs.getCorner(6);
			edge_axis_l[1] = rhs.getCorner(4);
			edge_axis_l[2] = rhs.getCorner(5);
			edge_axis_l[3] = rhs.getCorner(7);
			edge_axis_l[4] = rhs.getCorner(6) - rhs.getCorner(5);
			edge_axis_l[5] = rhs.getCorner(7) - rhs.getCorner(5);

			Vector_T<T, 3> edge_axis_r[6];
			edge_axis_r[0] = lhs.getCorner(6);
			edge_axis_r[1] = lhs.getCorner(4);
			edge_axis_r[2] = lhs.getCorner(5);
			edge_axis_r[3] = lhs.getCorner(7);
			edge_axis_r[4] = lhs.getCorner(6) - lhs.getCorner(5);
			edge_axis_r[5] = lhs.getCorner(7) - lhs.getCorner(5);

			for (int i = 0; i < 6; ++i)
			{
				for (int j = 0; j < 6; ++j)
				{
					Vector_T<T, 3> Axis = cross(edge_axis_l[i], edge_axis_r[j]);

					T min_l, max_l, min_r, max_r;
					min_l = max_l = dot(Axis, rhs.getCorner(0));
					min_r = max_r = dot(Axis, lhs.getCorner(0));
					for (int k = 1; k < 8; ++k)
					{
						T tmp = dot(Axis, rhs.getCorner(k));
						min_l = std::min(min_l, tmp);
						max_l = std::max(max_l, tmp);

						tmp = dot(Axis, lhs.getCorner(k));
						min_r = std::min(min_r, tmp);
						max_r = std::max(max_r, tmp);
					}
					outside |= min_l > max_r;
					outside |= min_r > max_l;
				}
			}
			if (outside)
			{
				return BO_No;
			}
			return BO_Partial;
		}

		template Color negative(Color const & rhs) AIR_NOEXCEPT;

		template<typename T>
		Color_T<T>	negative(Color_T<T> const & rhs) AIR_NOEXCEPT
		{
			return Color_T<T>(1 - rhs.r(), 1 - rhs.g(), 1 - rhs.b(), rhs.a());
		}

		template Color modulate(Color const & lhs, Color const & rhs) AIR_NOEXCEPT;

		template<typename T>
		Color_T<T> modulate(Color_T<T> const & lhs, Color_T<T> const & rhs) AIR_NOEXCEPT
		{
			return Color_T<T>(lhs.r() * rhs.r(), lhs.g() * rhs.g(), lhs.b() * rhs.b(), lhs.a() * rhs.a());
		}

		template
		Matrix4_T<float> scaling(float const & sx, float const & sy, float const & sz) AIR_NOEXCEPT;

		template
		Matrix4_T<float> scaling(Vector_T<float, 3> const & s) AIR_NOEXCEPT;

		template<typename T>
		Matrix4_T<T> scaling(T const & sx, T const & sy, T const & sz) AIR_NOEXCEPT
		{
			return Matrix4_T<T>(
				sx, 0, 0, 0,
				0, sy, 0, 0,
				0, 0, sz, 0,
				0, 0, 0, 1
				);
		}

		template<typename T>
		Matrix4_T<T> scaling(Vector_T<T, 3> const & s) AIR_NOEXCEPT
		{
			return Matrix4_T<T>(
				s.x(), 0, 0, 0,
				0, s.y(), 0, 0,
				0, 0, s.z(), 0,
				0, 0, 0, 1
				);
		}

		template
		float4x4 translation(float const & tx, float const & ty, float const & tz) AIR_NOEXCEPT;

		template<typename T>
		Matrix4_T<T> translation(T const & tx, T const & ty, T const & tz) AIR_NOEXCEPT
		{
			return Matrix4_T<T>(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				tx, ty, tz, 1
				);
		}

		template
		float4x4 translation(float3 const & t) AIR_NOEXCEPT;

		template<typename T>
		Matrix4_T<T> translation(Vector_T<T, 3> const & t) AIR_NOEXCEPT
		{
			return Matrix4_T<T>(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				t.x(), t.y(), t.z(), 1
				);
		}

		template float4x4 look_at_lh(float3 const & vEye, float3 const & vAt) AIR_NOEXCEPT;

		template<typename T>
		Matrix4_T<T> look_at_lh(Vector_T<T, 3> const &vEye, Vector_T<T, 3> const & vAt) AIR_NOEXCEPT
		{
			return look_at_lh(vEye, vAt, Vector_T<T, 3>(0, 1, 0));
		}

		template float4x4 look_at_lh(float3 const & vEye, float3 const & vAt, float3 const &vUp) AIR_NOEXCEPT;

		template<typename T>
		Matrix4_T<T> look_at_lh(Vector_T<T, 3> const & vEye, Vector_T<T, 3> const & vAt, Vector_T<T, 3> const &vUp) AIR_NOEXCEPT
		{
			Vector_T<T, 3> zAxis(normalize(vAt - vEye));
			Vector_T<T, 3> xAxis(normalize(cross(vUp, zAxis)));
			Vector_T<T, 3> yAxis(cross(zAxis, xAxis));
			return Matrix4_T<T>(
				xAxis.x(), yAxis.x(), zAxis.x(), 0,
				xAxis.y(), yAxis.y(), zAxis.y(), 0,
				xAxis.z(), yAxis.z(), zAxis.z(), 0,
				-dot(xAxis, vEye), -dot(yAxis, vEye), -dot(zAxis, vEye), 1);
		}
		template float determinant(float4x4 const & rhs) AIR_NOEXCEPT;

		template <typename T>
		T determinant(Matrix4_T<T> const & rhs) AIR_NOEXCEPT
		{
			T const _3142_3241(rhs(2, 0) * rhs(3, 1) - rhs(2, 1) * rhs(3, 0));
			T const _3143_3341(rhs(2, 0) * rhs(3, 2) - rhs(2, 2) * rhs(3, 0));
			T const _3144_3441(rhs(2, 0) * rhs(3, 3) - rhs(2, 3) * rhs(3, 0));
			T const _3243_3342(rhs(2, 1) * rhs(3, 2) - rhs(2, 2) * rhs(3, 1));
			T const _3244_3442(rhs(2, 1) * rhs(3, 3) - rhs(2, 3) * rhs(3, 1));
			T const _3344_3443(rhs(2, 2) * rhs(3, 3) - rhs(2, 3) * rhs(3, 2));

			return rhs(0, 0) * (rhs(1, 1) * _3344_3443 - rhs(1, 2) * _3244_3442 + rhs(1, 3) * _3243_3342)
				- rhs(0, 1) * (rhs(1, 0) * _3344_3443 - rhs(1, 2) * _3144_3441 + rhs(1, 3) * _3143_3341)
				+ rhs(0, 2) * (rhs(1, 0) * _3244_3442 - rhs(1, 1) * _3144_3441 + rhs(1, 3) * _3142_3241)
				- rhs(0, 3) * (rhs(1, 0) * _3243_3342 - rhs(1, 1) * _3143_3341 + rhs(1, 2) * _3142_3241);
		}

		template float4x4 inverse(float4x4 const & rhs) AIR_NOEXCEPT;

		template <typename T>
		Matrix4_T<T> inverse(Matrix4_T<T> const & rhs) AIR_NOEXCEPT
		{
			T const _2132_2231(rhs(1, 0) * rhs(2, 1) - rhs(1, 1) * rhs(2, 0));
			T const _2133_2331(rhs(1, 0) * rhs(2, 2) - rhs(1, 2) * rhs(2, 0));
			T const _2134_2431(rhs(1, 0) * rhs(2, 3) - rhs(1, 3) * rhs(2, 0));
			T const _2142_2241(rhs(1, 0) * rhs(3, 1) - rhs(1, 1) * rhs(3, 0));
			T const _2143_2341(rhs(1, 0) * rhs(3, 2) - rhs(1, 2) * rhs(3, 0));
			T const _2144_2441(rhs(1, 0) * rhs(3, 3) - rhs(1, 3) * rhs(3, 0));
			T const _2233_2332(rhs(1, 1) * rhs(2, 2) - rhs(1, 2) * rhs(2, 1));
			T const _2234_2432(rhs(1, 1) * rhs(2, 3) - rhs(1, 3) * rhs(2, 1));
			T const _2243_2342(rhs(1, 1) * rhs(3, 2) - rhs(1, 2) * rhs(3, 1));
			T const _2244_2442(rhs(1, 1) * rhs(3, 3) - rhs(1, 3) * rhs(3, 1));
			T const _2334_2433(rhs(1, 2) * rhs(2, 3) - rhs(1, 3) * rhs(2, 2));
			T const _2344_2443(rhs(1, 2) * rhs(3, 3) - rhs(1, 3) * rhs(3, 2));
			T const _3142_3241(rhs(2, 0) * rhs(3, 1) - rhs(2, 1) * rhs(3, 0));
			T const _3143_3341(rhs(2, 0) * rhs(3, 2) - rhs(2, 2) * rhs(3, 0));
			T const _3144_3441(rhs(2, 0) * rhs(3, 3) - rhs(2, 3) * rhs(3, 0));
			T const _3243_3342(rhs(2, 1) * rhs(3, 2) - rhs(2, 2) * rhs(3, 1));
			T const _3244_3442(rhs(2, 1) * rhs(3, 3) - rhs(2, 3) * rhs(3, 1));
			T const _3344_3443(rhs(2, 2) * rhs(3, 3) - rhs(2, 3) * rhs(3, 2));

			// 行列式的值
			T const det(determinant(rhs));
			if (equal<T>(det, 0))
			{
				return rhs;
			}
			else
			{
				T invDet(T(1) / det);

				return Matrix4_T<T>(
					+invDet * (rhs(1, 1) * _3344_3443 - rhs(1, 2) * _3244_3442 + rhs(1, 3) * _3243_3342),
					-invDet * (rhs(0, 1) * _3344_3443 - rhs(0, 2) * _3244_3442 + rhs(0, 3) * _3243_3342),
					+invDet * (rhs(0, 1) * _2344_2443 - rhs(0, 2) * _2244_2442 + rhs(0, 3) * _2243_2342),
					-invDet * (rhs(0, 1) * _2334_2433 - rhs(0, 2) * _2234_2432 + rhs(0, 3) * _2233_2332),

					-invDet * (rhs(1, 0) * _3344_3443 - rhs(1, 2) * _3144_3441 + rhs(1, 3) * _3143_3341),
					+invDet * (rhs(0, 0) * _3344_3443 - rhs(0, 2) * _3144_3441 + rhs(0, 3) * _3143_3341),
					-invDet * (rhs(0, 0) * _2344_2443 - rhs(0, 2) * _2144_2441 + rhs(0, 3) * _2143_2341),
					+invDet * (rhs(0, 0) * _2334_2433 - rhs(0, 2) * _2134_2431 + rhs(0, 3) * _2133_2331),

					+invDet * (rhs(1, 0) * _3244_3442 - rhs(1, 1) * _3144_3441 + rhs(1, 3) * _3142_3241),
					-invDet * (rhs(0, 0) * _3244_3442 - rhs(0, 1) * _3144_3441 + rhs(0, 3) * _3142_3241),
					+invDet * (rhs(0, 0) * _2244_2442 - rhs(0, 1) * _2144_2441 + rhs(0, 3) * _2142_2241),
					-invDet * (rhs(0, 0) * _2234_2432 - rhs(0, 1) * _2134_2431 + rhs(0, 3) * _2132_2231),

					-invDet * (rhs(1, 0) * _3243_3342 - rhs(1, 1) * _3143_3341 + rhs(1, 2) * _3142_3241),
					+invDet * (rhs(0, 0) * _3243_3342 - rhs(0, 1) * _3143_3341 + rhs(0, 2) * _3142_3241),
					-invDet * (rhs(0, 0) * _2243_2342 - rhs(0, 1) * _2143_2341 + rhs(0, 2) * _2142_2241),
					+invDet * (rhs(0, 0) * _2233_2332 - rhs(0, 1) * _2133_2331 + rhs(0, 2) * _2132_2231));
			}
		}

		template float4x4 perspective_fov_lh(float const & fov, float const & aspect, float const & nearPlane, float const & farPlane) AIR_NOEXCEPT;

		template <typename T>
		Matrix4_T<T> perspective_fov_lh(T const & fov, T const & aspect, T const & nearPlane, T const & farPlane) AIR_NOEXCEPT
		{
			T const h(T(1) / tan(fov / 2));
			T const w(h / aspect);
			T const q(farPlane / (farPlane - nearPlane));

			return Matrix4_T<T>(
				w, 0, 0, 0,
				0, h, 0, 0,
				0, 0, q, 1,
				0, 0, -nearPlane * q, 0);
		}

		template float4x4 ortho_lh(float const & w, float const & h, float const & nearPlane, float const & farPlane) AIR_NOEXCEPT;

		template <typename T>
		Matrix4_T<T> ortho_lh(T const & w, T const & h, T const & nearPlane, T const & farPlane) AIR_NOEXCEPT
		{
			T const w_2(w / 2);
			T const h_2(h / 2);
			return ortho_off_center_lh(-w_2, w_2, -h_2, h_2, nearPlane, farPlane);
		}

		template float4x4 ortho_off_center_lh(float const & left, float const & right, float const & bottom, float const & top,
			float const & nearPlane, float const & farPlane) AIR_NOEXCEPT;

		template <typename T>
		Matrix4_T<T> ortho_off_center_lh(T const & left, T const & right, T const & bottom, T const & top,
			T const & nearPlane, T const & farPlane) AIR_NOEXCEPT
		{
			T const q(T(1) / (farPlane - nearPlane));
			T const invWidth(T(1) / (right - left));
			T const invHeight(T(1) / (top - bottom));

			return Matrix4_T<T>(
				invWidth + invWidth, 0, 0, 0,
				0, invHeight + invHeight, 0, 0,
				0, 0, q, 0,
				-(left + right) * invWidth, -(top + bottom) * invHeight, -nearPlane * q, 1);
		}

		template float perspective_area(float3 const & view_pos, float4x4 const & view_proj, AABBox const & aabbox) AIR_NOEXCEPT;

		template <typename T>
		T perspective_area(Vector_T<T, 3> const & view_pos, Matrix4_T<T> const & view_proj, AABBox_T<T> const & aabbox) AIR_NOEXCEPT
		{
			static uint32_t const HULL_VERTEX[64][7] =
			{
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 4, 7, 3, 0, 0, 4 },
				{ 1, 2, 6, 5, 0, 0, 4 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 5, 4, 0, 0, 4 },
				{ 0, 1, 5, 4, 7, 3, 6 },
				{ 0, 1, 2, 6, 5, 4, 6 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 2, 3, 7, 6, 0, 0, 4 },
				{ 4, 7, 6, 2, 3, 0, 6 },
				{ 2, 3, 7, 6, 5, 1, 6 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 3, 2, 1, 0, 0, 4 },
				{ 0, 4, 7, 3, 2, 1, 6 },
				{ 0, 3, 2, 6, 5, 1, 6 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 3, 2, 1, 5, 4, 6 },
				{ 2, 1, 5, 4, 7, 3, 6 },
				{ 0, 3, 2, 6, 5, 4, 6 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 3, 7, 6, 2, 1, 6 },
				{ 0, 4, 7, 6, 2, 1, 6 },
				{ 0, 3, 7, 6, 5, 1, 6 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 4, 5, 6, 7, 0, 0, 4 },
				{ 4, 5, 6, 7, 3, 0, 6 },
				{ 1, 2, 6, 7, 4, 5, 6 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 5, 6, 7, 4, 6 },
				{ 0, 1, 5, 6, 7, 3, 6 },
				{ 0, 1, 2, 6, 7, 4, 6 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 2, 3, 7, 4, 5, 6, 6 },
				{ 0, 4, 5, 6, 2, 3, 6 },
				{ 1, 2, 3, 7, 4, 5, 6 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 }
			};

			uint32_t const pos = ((view_pos.x() < aabbox.getMin().x()))
				| ((view_pos.x() > aabbox.getMax().x()) << 1)
				| ((view_pos.y() < aabbox.getMin().y()) << 2)
				| ((view_pos.y() > aabbox.getMax().y()) << 3)
				| ((view_pos.z() < aabbox.getMin().z()) << 4)
				| ((view_pos.z() > aabbox.getMax().z()) << 5);
			if (0 == pos)
			{
				return 1;
			}
			uint32_t const num = HULL_VERTEX[pos][6];
			if (0 == num)
			{
				return 0;
			}

			Vector_T<T, 3> dst[8];
			for (uint32_t i = 0; i < num; ++i)
			{
				dst[i] = MathLib::transform_coord(aabbox.getCorner(HULL_VERTEX[pos][i]), view_proj);
				dst[i] = dst[i] * T(0.5) + Vector_T<T, 3>(0.5, 0.5, 0.0);
			}

			T sum = 0;
			for (uint32_t i = 0; i < num; ++i)
			{
				int next = (i + 1) % num;
				sum += abs((dst[i].x() - dst[next].x()) * (dst[i].y() + dst[next].y()));
			}
			return sum / 2;
		}

		template float ortho_area(float3 const & view_dir, AABBox const & aabb) AIR_NOEXCEPT;

		template<typename T>
		T ortho_area(Vector_T<T, 3> const & view_dir, AABBox_T<T> const & aabb) AIR_NOEXCEPT
		{
			Vector_T<T, 3> size = aabb.getMax() - aabb.getMin();
			return dot(Vector_T<T, 3>(abs(view_dir.x()), abs(view_dir.y
			()), abs(view_dir.z())), Vector_T<T, 3>(size.y() * size
				.z(), size.z() * size.x(), size.x() * size.y()));
		}
	}
}