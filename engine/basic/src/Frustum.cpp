#include "basic/include/Basic.h"
#include "basic/include/Bound.hpp"
#include "basic/include/Vector.hpp"
#include "basic/include/Matrix.hpp"
#include "basic/include/Frustum.hpp"

namespace Air
{
	template Frustum_T<float>::Frustum_T(Frustum const & rhs) AIR_NOEXCEPT;
	template Frustum_T<float>::Frustum_T(Frustum && rhs) AIR_NOEXCEPT;
	template Frustum& Frustum_T<float>::operator=(Frustum const & rhs) AIR_NOEXCEPT;
	template Frustum& Frustum_T<float>::operator=(Frustum&& rhs) AIR_NOEXCEPT;
	template void Frustum_T<float>::clipMatrix(float4x4 const & clip, float4x4 const & inv_clip) AIR_NOEXCEPT;
	template bool Frustum_T<float>::isEmpty() const AIR_NOEXCEPT;
	template bool Frustum_T<float>::contains(float3 const & v) const AIR_NOEXCEPT;
	template float Frustum_T<float>::getMaxRadiusSq() const AIR_NOEXCEPT;
	template BoundOverlap Frustum_T<float>::intersect(AABBox const & aabb) const AIR_NOEXCEPT;
	template BoundOverlap Frustum_T<float>::intersect(Frustum const & frustum) const AIR_NOEXCEPT;
	template Plane const & Frustum_T<float>::getFrustumPlane(uint32_t index) const AIR_NOEXCEPT;
	template Vector_T<float, 3> const & Frustum_T<float>::getCorner(uint32_t index) const AIR_NOEXCEPT;


	template<typename T>
	Frustum_T<T>::Frustum_T(Frustum_T<T> const & rhs) AIR_NOEXCEPT
		:mPlanes(rhs.mPlanes), mCorners(rhs.mCorners)
	{
	}


	template<typename T>
	Frustum_T<T>::Frustum_T(Frustum_T<T> && rhs) AIR_NOEXCEPT
		: mPlanes(std::move(rhs.mPlanes)), mCorners(std::move(rhs.mCorners))
	{

	}

	template<typename T>
	Frustum_T<T>& Frustum_T<T>::operator= (Frustum_T<T> const & rhs) AIR_NOEXCEPT
	{
		if (this != &rhs)
		{
			mPlanes = rhs.mPlanes;
			mCorners = rhs.mCorners;
		}
		return *this;
	}

	template<typename T>
	Frustum_T<T>& Frustum_T<T>::operator= (Frustum_T<T> && rhs) AIR_NOEXCEPT
	{
		mPlanes = std::move(rhs.mPlanes);
		mCorners = std::move(rhs.mCorners);
		return *this;
	}

	template<typename T>
	void Frustum_T<T>::clipMatrix(Matrix4_T<T> const & clip, Matrix4_T<T> const & inv_clip) AIR_NOEXCEPT
	{
		mCorners[0] = MathLib::transform_coord(Vector_T<T, 3>(-1, -1, 0), inv_clip);
		mCorners[1] = MathLib::transform_coord(Vector_T<T, 3>(1, -1, 0), inv_clip);
		mCorners[2] = MathLib::transform_coord(Vector_T<T, 3>(-1, 1, 0), inv_clip);
		mCorners[3] = MathLib::transform_coord(Vector_T<T, 3>(1, 1, 0), inv_clip);

		mCorners[4] = MathLib::transform_coord(Vector_T<T, 3>(-1, -1, 1), inv_clip);
		mCorners[5] = MathLib::transform_coord(Vector_T<T, 3>(1, -1, 1), inv_clip);
		mCorners[6] = MathLib::transform_coord(Vector_T<T, 3>(-1, 1, 1), inv_clip);
		mCorners[7] = MathLib::transform_coord(Vector_T<T, 3>(1, 1, 1), inv_clip);

		Vector_T<T, 4> const & column1(clip.col(0));
		Vector_T<T, 4> const & column2(clip.col(1));
		Vector_T<T, 4> const & column3(clip.col(2));
		Vector_T<T, 4> const & column4(clip.col(3));

		mPlanes[0] = column4 - column1;
		mPlanes[1] = column4 + column1;
		mPlanes[2] = column4 - column2;
		mPlanes[3] = column4 + column2;
		mPlanes[4] = column4 - column3;
		mPlanes[5] = column3;

		for (auto & plane : mPlanes)
		{
			plane = MathLib::normalize(plane);
		}
	}

	template<typename T>
	bool Frustum_T<T>::isEmpty() const AIR_NOEXCEPT
	{
		return false;
	}

	template<typename T>
	bool Frustum_T<T>::contains(Vector_T<T, 3> const  & v) const AIR_NOEXCEPT
	{
		return MathLib::intersect_point_frustum(v, *this);
	}

	template<typename T>
	T Frustum_T<T>::getMaxRadiusSq() const AIR_NOEXCEPT
	{
		return 0;
	}
	template<typename T>
	void Frustum_T<T>::setFrustumPlane(uint32_t index, Plane_T<T> const & plane) AIR_NOEXCEPT
	{
		mPlanes[index] = plane;
	}


	template<typename T>
	Plane_T<T> const & Frustum_T<T>::getFrustumPlane(uint32_t index) const AIR_NOEXCEPT
	{
		return mPlanes[index];
	}

	template<typename T>
	void Frustum_T<T>::setCorner(uint32_t index, Vector_T<T, 3> const & corner) AIR_NOEXCEPT
	{
		mCorners[index] = corner;
	}

	template<typename T>
	Vector_T<T, 3> const & Frustum_T<T>::getCorner(uint32_t index) const AIR_NOEXCEPT
	{
		return mCorners[index];
	}

	template<typename T>
	BoundOverlap Frustum_T<T>::intersect(AABBox_T<T> const & aabb) const AIR_NOEXCEPT
	{
		return MathLib::intersect_aabb_frustum(aabb, *this);
	}

	template<typename T>
	BoundOverlap Frustum_T<T>::intersect(Frustum_T<T> const & frustum) const AIR_NOEXCEPT
	{
		return MathLib::intersect_frustum_frustum(frustum, *this);
	}
}


