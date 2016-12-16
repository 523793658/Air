#ifndef _AABBBox_H_
#define _AABBBox_H_
#pragma once

#include "boost/operators.hpp"
#include "PreDeclare.h"
#include "Bound.hpp"

namespace Air
{
	class AABBox : boost::addable2<AABBox, float3,
		boost::subtractable2<AABBox, float3,
		boost::multipliable2<AABBox, float, 
		boost::dividable2<AABBox, float,
		boost::andable<AABBox,
		boost::orable<AABBox,
		boost::equality_comparable<AABBox>>>>>>>,
		public Bound_T
	{
		public:
			AABBox() AIR_NOEXCEPT
			{

			}

			AABBox(float3 const & vMin, float3 const & vMax) AIR_NOEXCEPT;
			AABBox(float3 && vMin, float3 && vMax) AIR_NOEXCEPT;
			AABBox(AABBox const & rhs) AIR_NOEXCEPT;
			AABBox(AABBox && rh) AIR_NOEXCEPT;

			AABBox& operator += (float3 const & rhs) AIR_NOEXCEPT;
			AABBox& operator -= (float3 const & rhs) AIR_NOEXCEPT;
			AABBox& operator *= (float rhs) AIR_NOEXCEPT;
			AABBox& operator /= (float rhs)	AIR_NOEXCEPT;
			AABBox& operator &= (AABBox const & rhs) AIR_NOEXCEPT;
			AABBox& operator |= (AABBox const & rhs) AIR_NOEXCEPT;

			AABBox& operator = (AABBox const & rhs) AIR_NOEXCEPT;
			AABBox& operator = (AABBox && rhs) AIR_NOEXCEPT;

			AABBox const operator+() const AIR_NOEXCEPT;
			AABBox const operator-() const AIR_NOEXCEPT;

			float getWidth() const AIR_NOEXCEPT;
			float getHeight() const AIR_NOEXCEPT;
			float getDepth() const AIR_NOEXCEPT;

			float3 const getLeftBottomNear() const AIR_NOEXCEPT;
			float3 const getLeftTopNear() const AIR_NOEXCEPT;
			float3 const getRightBottomNear() const AIR_NOEXCEPT;
			float3 const getRightTopNear() const AIR_NOEXCEPT;
			float3 const getLeftBottomFar() const AIR_NOEXCEPT;
			float3 const getLeftTopFar() const AIR_NOEXCEPT;
			float3 const getRightBottomFar() const AIR_NOEXCEPT;
			float3 const getRightTopFar() const AIR_NOEXCEPT;

			float3& getMin() AIR_NOEXCEPT;
			float3 const & getMin() const AIR_NOEXCEPT;

			float3& getMax() AIR_NOEXCEPT;
			float3 const & getMax() const AIR_NOEXCEPT;

			float3 getCenter() const AIR_NOEXCEPT;
			float3 getHalfSize() const AIR_NOEXCEPT;

			bool Intersect(AABBox const & aabb) AIR_NOEXCEPT;
			bool Intersect(Frustum const & frustum) AIR_NOEXCEPT;


			virtual bool isEmpty() const AIR_NOEXCEPT override;
			virtual float getMaxRadiusSq() const AIR_NOEXCEPT override;
			virtual bool contains(float3 const & v) const AIR_NOEXCEPT override;
	private:
		float3 mMin, mMax;
	};
}
#endif