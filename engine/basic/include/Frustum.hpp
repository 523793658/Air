#ifndef _Frustum_H_
#define _Frustum_H_
#include "basic/include/Bound.hpp"
#include "basic/include/AABBBox.hpp"
namespace Air
{

#ifndef FLAMEMATH
	class Frustum : public Bound_T
	{
	public:
		Frustum() AIR_NOEXCEPT
		{

		}

		Frustum(Frustum const & rhs) AIR_NOEXCEPT;
		Frustum(Frustum && rhs) AIR_NOEXCEPT;

		Frustum& operator= (Frustum const & rhs) AIR_NOEXCEPT;
		Frustum& operator= (Frustum && rhs) AIR_NOEXCEPT;

		void clipMatrix(float4x4 const & clip, float4x4 const & inv_clip) AIR_NOEXCEPT;

		virtual bool isEmpty() const AIR_NOEXCEPT override;
		virtual bool verInBound(float3 const & v) const AIR_NOEXCEPT override;
		virtual float getMaxRadiusSq() const AIR_NOEXCEPT override;

		void setFrustumPlane(uint32_t index, Plane const & plane) AIR_NOEXCEPT;

		Plane const & getFrustumPlane(uint32_t index) const AIR_NOEXCEPT;

		void setCorner(uint32_t index, float3 const & corner) AIR_NOEXCEPT;

		float3 const & getCorner(uint32_t index) const AIR_NOEXCEPT;

		BoundOverlap intersect(AABBox const & aabb) const AIR_NOEXCEPT;
		BoundOverlap intersect(Frustum const & frustum) const AIR_NOEXCEPT;

	private:
		std::array<Plane, 6> mPlanes;
		std::array<float3, 8> mCorners;
	};
#else
	typedef BoundingFrustum Frustum;
#endif


	
}
#endif