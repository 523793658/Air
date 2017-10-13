#ifndef _AABBBox_H_
#define _AABBBox_H_
#pragma once

#include "basic/include/PreDeclare.h"
#include "boost/operators.hpp"
#include "Bound.hpp"

namespace Air
{
	template<typename T>
	class AABBox_T : boost::addable2<AABBox_T<T>, Vector_T<T,3>,
		boost::subtractable2<AABBox_T<T>, Vector_T<T,3>,
		boost::multipliable2<AABBox_T<T>, T, 
		boost::dividable2<AABBox_T<T>, T,
		boost::andable<AABBox_T<T>,
		boost::orable<AABBox_T<T>,
		boost::equality_comparable<AABBox_T<T>>>>>>>>,
		public Bound_T<T>
	{
		public:
			AABBox_T() AIR_NOEXCEPT;

			AABBox_T(Vector_T<T,3> const & vMin, Vector_T<T,3> const & vMax) AIR_NOEXCEPT;
			AABBox_T(Vector_T<T,3> && vMin, Vector_T<T,3> && vMax) AIR_NOEXCEPT;
			AABBox_T(AABBox_T const & rhs) AIR_NOEXCEPT;
			AABBox_T(AABBox_T && rh) AIR_NOEXCEPT;

			AABBox_T& operator += (Vector_T<T,3> const & rhs) AIR_NOEXCEPT;
			AABBox_T& operator -= (Vector_T<T,3> const & rhs) AIR_NOEXCEPT;
			AABBox_T& operator *= (float rhs) AIR_NOEXCEPT;
			AABBox_T& operator /= (float rhs)	AIR_NOEXCEPT;
			AABBox_T& operator &= (AABBox_T const & rhs) AIR_NOEXCEPT;
			AABBox_T& operator |= (AABBox_T const & rhs) AIR_NOEXCEPT;

			AABBox_T& operator = (AABBox_T const & rhs) AIR_NOEXCEPT;
			AABBox_T& operator = (AABBox_T && rhs) AIR_NOEXCEPT;

			AABBox_T const operator+() const AIR_NOEXCEPT;
			AABBox_T const operator-() const AIR_NOEXCEPT;

			float getWidth() const AIR_NOEXCEPT;
			float getHeight() const AIR_NOEXCEPT;
			float getDepth() const AIR_NOEXCEPT;

			Vector_T<T,3> const getLeftBottomNear() const AIR_NOEXCEPT;
			Vector_T<T,3> const getLeftTopNear() const AIR_NOEXCEPT;
			Vector_T<T,3> const getRightBottomNear() const AIR_NOEXCEPT;
			Vector_T<T,3> const getRightTopNear() const AIR_NOEXCEPT;
			Vector_T<T,3> const getLeftBottomFar() const AIR_NOEXCEPT;
			Vector_T<T,3> const getLeftTopFar() const AIR_NOEXCEPT;
			Vector_T<T,3> const getRightBottomFar() const AIR_NOEXCEPT;
			Vector_T<T,3> const getRightTopFar() const AIR_NOEXCEPT;

			Vector_T<T,3>& getMin() AIR_NOEXCEPT;
			Vector_T<T,3> const & getMin() const AIR_NOEXCEPT;

			Vector_T<T,3>& getMax() AIR_NOEXCEPT;
			Vector_T<T,3> const & getMax() const AIR_NOEXCEPT;

			Vector_T<T,3> getCenter() const AIR_NOEXCEPT;
			Vector_T<T,3> getHalfSize() const AIR_NOEXCEPT;

			bool Intersect(AABBox_T const & aabb) AIR_NOEXCEPT;
			bool Intersect(Frustum_T<T> const & frustum) AIR_NOEXCEPT;

			Vector_T<T, 3> getCorner(size_t index) const AIR_NOEXCEPT;

			void join(Vector_T<T, 3> const & point) AIR_NOEXCEPT;

			void join(AABBox_T const & aabb) AIR_NOEXCEPT;


			virtual bool isEmpty() const AIR_NOEXCEPT override;
			virtual float getMaxRadiusSq() const AIR_NOEXCEPT override;
			virtual bool contains(Vector_T<T, 3> const & v) const AIR_NOEXCEPT override;

			bool contains(AABBox_T<T> const & aabb) const AIR_NOEXCEPT;
	private:
		Vector_T<T,3> mMin, mMax;
	};
}
#endif