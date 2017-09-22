#ifndef _Frustum_H_
#define _Frustum_H_

#include "basic/include/PreDeclare.h"
#include "array"

namespace Air
{
	template<typename T>
	class Frustum_T : public Bound_T<T>
	{
	public:
		Frustum_T() AIR_NOEXCEPT
		{

		}

		Frustum_T(Frustum_T const & rhs) AIR_NOEXCEPT;
		Frustum_T(Frustum_T && rhs) AIR_NOEXCEPT;

		Frustum_T& operator= (Frustum_T const & rhs) AIR_NOEXCEPT;
		Frustum_T& operator= (Frustum_T && rhs) AIR_NOEXCEPT;

		void clipMatrix(Matrix4_T<T> const & clip, Matrix4_T<T> const & inv_clip) AIR_NOEXCEPT;

		virtual bool isEmpty() const AIR_NOEXCEPT override;
		virtual bool contains(Vector_T<T, 3> const & v) const AIR_NOEXCEPT override;
		virtual T getMaxRadiusSq() const AIR_NOEXCEPT override;

		void setFrustumPlane(uint32_t index, Plane_T<T> const & plane) AIR_NOEXCEPT;

		Plane_T<T> const & getFrustumPlane(uint32_t index) const AIR_NOEXCEPT;

		void setCorner(uint32_t index, Vector_T<T, 3> const & corner) AIR_NOEXCEPT;

		Vector_T<T, 3> const & getCorner(uint32_t index) const AIR_NOEXCEPT;

		BoundOverlap intersect(AABBox_T<T> const & aabb) const AIR_NOEXCEPT;
		BoundOverlap intersect(Frustum_T const & frustum) const AIR_NOEXCEPT;

	private:
		std::array<Plane_T<T>, 6> mPlanes;
		std::array<Vector_T<T, 3>, 8> mCorners;
	};


	
}
#endif