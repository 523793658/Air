#include "basic/include/Basic.h"
#include "basic/include/Frustum.hpp"

namespace Air
{

#ifndef FLAMEMATH
	Frustum::Frustum(Frustum const & rhs) AIR_NOEXCEPT
		:mPlanes(rhs.mPlanes), mCorners(rhs.mCorners)
	{
	}
	Frustum::Frustum(Frustum && rhs) AIR_NOEXCEPT
		: mPlanes(std::move(rhs.mPlanes)), mCorners(std::move(rhs.mCorners))
	{

	}

	Frustum& Frustum::operator= (Frustum const & rhs) AIR_NOEXCEPT
	{
		if (this != &rhs)
		{
			mPlanes = rhs.mPlanes;
			mCorners = rhs.mCorners;
		}
		return *this;
	}
	Frustum& Frustum::operator= (Frustum && rhs) AIR_NOEXCEPT
	{
		mPlanes = std::move(rhs.mPlanes);
		mCorners = std::move(rhs.mCorners);
		return *this;
	}

	void Frustum::clipMatrix(float4x4 const & clip, float4x4 const & inv_clip) AIR_NOEXCEPT
	{
		mCorners[0] = float3::Transform(float3(-1.0f, -1.0f, 0.0f), inv_clip);
		mCorners[1] = float3::Transform(float3(+1.0f, -1.0f, 0.0f), inv_clip);
		mCorners[2] = float3::Transform(float3(-1.0f, +1.0f, 0.0f), inv_clip);
		mCorners[3] = float3::Transform(float3(+1.0f, +1.0f, 0.0f), inv_clip);
		mCorners[4] = float3::Transform(float3(-1.0f, -1.0f, 1.0f), inv_clip);
		mCorners[5] = float3::Transform(float3(+1.0f, -1.0f, 1.0f), inv_clip);
		mCorners[6] = float3::Transform(float3(-1.0f, +1.0f, 1.0f), inv_clip);
		mCorners[7] = float3::Transform(float3(+1.0f, +1.0f, 1.0f), inv_clip);

		float4 const c1(clip._11, clip._21, clip._32, clip._41);
		float4 const c2(clip._12, clip._22, clip._32, clip._42);
		float4 const c3(clip._13, clip._23, clip._33, clip._43);
		float4 const c4(clip._14, clip._24, clip._34, clip._44);

		mPlanes[0] = Plane(c4 - c1);
		mPlanes[0].Normalize();
		mPlanes[1] = Plane(c4 + c1);
		mPlanes[1].Normalize();
		mPlanes[2] = Plane(c4 - c2);
		mPlanes[2].Normalize();
		mPlanes[3] = Plane(c4 + c2);
		mPlanes[3].Normalize();
		mPlanes[4] = Plane(c4 - c3);
		mPlanes[4].Normalize();
		mPlanes[5] = Plane(c4 + c3);
		mPlanes[5].Normalize();

	}

	bool Frustum::isEmpty() const AIR_NOEXCEPT
	{
		return false;
	}
	bool Frustum::verInBound(float3 const & v) const AIR_NOEXCEPT
	{
		for (int i = 0; i < 6; i++)
		{
			if (mPlanes[i].DotCoordinate(v) < 0)
			{
				return false;
			}
		}
		return true;
	}
	float Frustum::getMaxRadiusSq() const AIR_NOEXCEPT
	{
		return 0;
	}

	void Frustum::setFrustumPlane(uint32_t index, Plane const & plane) AIR_NOEXCEPT
	{
		mPlanes[index] = plane;
	}

	Plane const & Frustum::getFrustumPlane(uint32_t index) const AIR_NOEXCEPT
	{
		return mPlanes[index];
	}

	void Frustum::setCorner(uint32_t index, float3 const & corner) AIR_NOEXCEPT
	{
		mCorners[index] = corner;
	}

	float3 const & Frustum::getCorner(uint32_t index) const AIR_NOEXCEPT
	{
		return mCorners[index];
	}

	BoundOverlap Frustum::intersect(AABBox const & aabb) const AIR_NOEXCEPT
	{
		float3 const & min_pt = aabb.
	}
	BoundOverlap intersect(Frustum const & frustum) const AIR_NOEXCEPT;

#else

#endif
}


