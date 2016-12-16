#include "basic/include/Basic.h"
#include "FlameMath.h"
#include "basic/include/Frustum.hpp"
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

		BoundOverlap intersect_aabb_frustum(AABBox const aabb, Frustum const & frustum)
		{
			float3 const & minPt = aabb.getMin();
			float3 const & maxPt = aabb.getMax();

			bool intersect = false;
			Plane planes[6];
			frustum.GetPlanes(&planes[0], &planes[1], &planes[2], &planes[3], &planes[4], &planes[5], &planes[6]);
			for (int i = 0; i < 6; i++)
			{
			}
		}

		BoundOverlap perspective_area(float3 eye_pos, float4x4 view_proj, AABBox const & aabbox)
		{
			if (aabbox.contains(eye_pos))
			{
				return BO_Yes;
			}
			
			Frustum frustum;
			Frustum::CreateFromMatrix(frustum, view_proj);
#ifdef FLAMEMATH
			return static_cast<BoundOverlap>(frustum.Contains(AABB(aabbox.getCenter(), aabbox.getHalfSize())));
#endif
		}

#ifdef FLAMEMATH

#else

		float4x4 scaling(float const & sx, float const & sy, float const & sz)AIR_NOEXCEPT
		{
			return float4x4(
				sx,		0.0f,	0.0f,	0.0f,
				0.0f,	 sy,	0.0f,	0.0f,
				0.0f,	 0.0f,	sz,		0.0f,
				0.0f,	 0.0f,	0.0f,	1.0f
				);
		}
		float4x4 translation(float const & tx, float const & ty, float const &tz)AIR_NOEXCEPT
		{
			return float4x4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				tx,	  ty,   tz,   1.0		
				);
		}

		float4x4 buildViewMatrixLH(float3 const & vEye, float3 const & vAt, float3 const & vUp)AIR_NOEXCEPT
		{
			float3 zAxis = vAt - vEye;
			zAxis.Normalize();
			float3 xAxis = vUp ^ zAxis;
			xAxis.Normalize();
			float3 yAxis = zAxis ^ xAxis;

			return float4x4(
				xAxis.x, yAxis.x, zAxis.x, 0.0f,
				xAxis.y, yAxis.y, zAxis.y, 0.0f,
				xAxis.z, yAxis.z, zAxis.z, 0.0f,
				-xAxis.Dot(vEye), -yAxis.Dot(vEye), -zAxis.Dot(vEye), 1.0f);
		}

		float4x4 buildProjMatrixLH(float fov, float aspect, float near_plane, float far_plane)AIR_NOEXCEPT
		{
			float const h(1.0f / tan(fov / 2));
			float const w(h / aspect);
			float const q(far_plane / (far_plane - near_plane));

			return float4x4(
				w, 0.0f, 0.0f, 0.0f,
				0.0f, h, 0.0f, 0.0f,
				0.0f, 0.0f, q, 1.0f,
				0.0f, 0.0f, -near_plane * q, 0.0f);
		}

		float4x4 buildProjMatrixLH(float const & left, float const & right, float const bottom, float const & top, float const &nearPlane, float const & farPlane)AIR_NOEXCEPT
		{
			float const q(farPlane / (farPlane - nearPlane));
			float const near2(nearPlane + nearPlane);
			float const invWidth(1 / (right - left));
			float const invHeight(1 / (top - bottom));

			return float4x4(
				near2 * invWidth,				0.0f,							0.0f,			0.0f,
				0.0f,							near2 * invHeight,				0.0f,			0.0f,
				-(left + right) * invWidth,		-(top + bottom) * invHeight,	q,				1.0f,
				0.0f,							0.0f,							-nearPlane*q,	0.0f
				);
		}

		float4x4 buildProjOrthoMatrixLH(float const & w, float const & h, float const & nearPlane, float const & farPlane) AIR_NOEXCEPT
		{
			float const w_2(w / 2.0f);
			float const h_2(h / 2.0f);
			return buildProjOrthoMatrixLH(-w_2, w_2, -h_2, h_2, nearPlane, farPlane);
		}

		float4x4 buildProjOrthoMatrixLH(float const & left, float const & right, float const & bottom, float const & top, float const & nearPlane, float const & farPlane) AIR_NOEXCEPT
		{
			float const q(1.0f / (farPlane - nearPlane));
			float const invWidth(1.0f / (right - left));
			float const invHeight(1.0f / (top - bottom));

			return float4x4(
				invWidth + invWidth,			0.0f,								0.0f,							0.0f,
				0.0f,							invHeight + invHeight,				0.0f,							0.0f,
				0.0f,							0.0f,								q,								0.0f,
				-(left + right) *invWidth,		-(top + bottom) * invHeight,		-nearPlane * q,					1.0f);
		}
#endif

	}
}