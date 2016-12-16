#include "basic/include/Basic.h"
#include "basic/include/Math.hpp"
#include "basic/include/AABBBox.hpp"
namespace Air{
	AABBox::AABBox(float3 const & vMin, float3 const & vMax) AIR_NOEXCEPT
	{
		mMin = vMin;
		mMax = vMax;
	}
	AABBox::AABBox(float3 && vMin, float3 && vMax) AIR_NOEXCEPT
	{
		mMax = std::move(vMax);
		mMin = std::move(vMin);
	}
	AABBox::AABBox(AABBox const & rhs) AIR_NOEXCEPT
	{
		mMax = rhs.mMax;
		mMin = rhs.mMin;
	}
	AABBox::AABBox(AABBox && rh) AIR_NOEXCEPT
	{
		mMin = std::move(rh.mMin);
		mMax = std::move(rh.mMax);
	}

	bool AABBox::isEmpty() const AIR_NOEXCEPT
	{
		return mMax == mMin;
	}
	bool AABBox::contains(float3 const & v) const AIR_NOEXCEPT
	{
		return MathLib::in_bound(v.x, mMin.x, mMax.x) && 
			MathLib::in_bound(v.y, mMin.y, mMax.y) &&
			MathLib::in_bound(v.z, mMin.z, mMax.z);
	}

	float AABBox::getMaxRadiusSq() const AIR_NOEXCEPT
	{
		return std::max<float>(mMax.Length(), mMin.Length());
	}

	

	AABBox& AABBox::operator += (float3 const & rhs) AIR_NOEXCEPT
	{
		mMin += rhs;
		mMax += rhs;
		return *this;
	}
	AABBox& AABBox::operator -= (float3 const & rhs) AIR_NOEXCEPT
	{
		mMin -= rhs;
		mMax -= rhs;
		return *this;
	}
	AABBox& AABBox::operator *= (float rhs) AIR_NOEXCEPT
	{
		mMin *= rhs;
		mMax *= rhs;
		return *this;
	}
	AABBox& AABBox::operator /= (float rhs)	AIR_NOEXCEPT
	{
		mMin /= rhs;
		mMax /= rhs;
		return *this;
	}
	AABBox& AABBox::operator &= (AABBox const & rhs) AIR_NOEXCEPT
	{
		mMin.x = std::max(rhs.mMin.x, mMin.x);
		mMin.y = std::max(rhs.mMin.y, mMin.y);
		mMin.z = std::max(rhs.mMin.z, mMin.z);


		mMax.x = std::min(rhs.mMax.x, mMax.x);
		mMax.y = std::min(rhs.mMax.y, mMax.y);
		mMax.z = std::min(rhs.mMax.z, mMax.z);
		return *this;
	}
	AABBox& AABBox::operator |= (AABBox const & rhs) AIR_NOEXCEPT
	{
		mMin.x = std::min(rhs.mMin.x, mMin.x);
		mMin.y = std::min(rhs.mMin.y, mMin.y);
		mMin.z = std::min(rhs.mMin.z, mMin.z);


		mMax.x = std::max(rhs.mMax.x, mMax.x);
		mMax.y = std::max(rhs.mMax.y, mMax.y);
		mMax.z = std::max(rhs.mMax.z, mMax.z);
		return *this;
	}

	AABBox& AABBox::operator = (AABBox const & rhs) AIR_NOEXCEPT
	{
		if (this != &rhs)
		{
			mMax = rhs.mMax;
			mMin = rhs.mMin;
		}
		return *this;
	}
	AABBox& AABBox::operator = (AABBox && rhs) AIR_NOEXCEPT
	{
		mMin = std::move(rhs.mMin);
		mMax = std::move(rhs.mMax);
	}

	AABBox const AABBox::operator+() const AIR_NOEXCEPT
	{
		return *this;
	}
	AABBox const AABBox::operator-() const AIR_NOEXCEPT
	{
		return AABBox(-this->mMax, -this->mMin);
	}

	float AABBox::getWidth() const AIR_NOEXCEPT
	{
		return mMax.x - mMin.x;
	}
	float AABBox::getHeight() const AIR_NOEXCEPT
	{
		return mMax.y - mMin.y;
	}
	float AABBox::getDepth() const AIR_NOEXCEPT
	{
		return mMax.z - mMin.z;
	}

	float3 const AABBox::getLeftBottomNear() const AIR_NOEXCEPT
	{
		return mMin;
	}
	float3 const AABBox::getLeftTopNear() const AIR_NOEXCEPT
	{
		return float3(mMin.x, mMax.y, mMin.z);
	}
	float3 const AABBox::getRightBottomNear() const AIR_NOEXCEPT
	{
		return float3(mMax.x, mMin.y, mMin.z);
	}
	float3 const AABBox::getRightTopNear() const AIR_NOEXCEPT
	{
		return float3(mMax.x, mMax.y, mMin.z);
	}
	float3 const AABBox::getLeftBottomFar() const AIR_NOEXCEPT
	{
		return float3(mMin.x, mMin.y, mMax.z);
	}
	float3 const AABBox::getLeftTopFar() const AIR_NOEXCEPT
	{
		return float3(mMin.x, mMax.y, mMax.z);
	}
	float3 const AABBox::getRightBottomFar() const AIR_NOEXCEPT
	{
		return float3(mMax.x, mMin.y, mMax.z);
	}
	float3 const AABBox::getRightTopFar() const AIR_NOEXCEPT
	{
		return this->mMax;
	}

	float3& AABBox::getMin() AIR_NOEXCEPT
	{
		return mMin;
	}
	float3 const & AABBox::getMin() const AIR_NOEXCEPT
	{
		return mMin;
	}

	float3& AABBox::getMax() AIR_NOEXCEPT
	{
		return mMax;
	}
	float3 const & AABBox::getMax() const AIR_NOEXCEPT
	{
		return mMin;
	}

	float3 AABBox::getCenter() const AIR_NOEXCEPT
	{
		return (mMin + mMax) / 2.0f;
	}
	float3 AABBox::getHalfSize() const AIR_NOEXCEPT
	{
		return (mMax - mMin) / 2.0f;
	}

	bool AABBox::Intersect(AABBox const & aabb) AIR_NOEXCEPT
	{
		float3 const t = this->getCenter() - aabb.getCenter();
		float3 const d = this->getHalfSize + aabb.getHalfSize();
		return (MathLib::abs(t.x) <= d.x) && (MathLib::abs(t.y) <= d.y) && (MathLib::abs(t.z) <= d.z);
	}
	bool Intersect(Frustum const & frustum) AIR_NOEXCEPT
	{
		
	}
}