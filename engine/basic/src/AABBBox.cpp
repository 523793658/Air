#include "basic/include/Basic.h"
#include "basic/include/Math.hpp"
#include "basic/include/Vector.hpp"
#include "boost/assert.hpp"
#include "basic/include/AABBBox.hpp"
namespace Air{

	template AABBox::AABBox_T(float3 const & vMin, float3 const & vMax) AIR_NOEXCEPT;
	template AABBox::AABBox_T(float3 && vMin, float3 && vMax) AIR_NOEXCEPT;
	template AABBox::AABBox_T(AABBox const & rhs) AIR_NOEXCEPT;
	template AABBox::AABBox_T(AABBox && rh) AIR_NOEXCEPT;
	template AABBox::AABBox_T() AIR_NOEXCEPT;

	template AABBox& AABBox::operator += (float3 const & rhs) AIR_NOEXCEPT;
	template AABBox& AABBox::operator -= (float3 const & rhs) AIR_NOEXCEPT;
	template AABBox& AABBox::operator *= (float rhs) AIR_NOEXCEPT;
	template AABBox& AABBox::operator /= (float rhs)	AIR_NOEXCEPT;
	template AABBox& AABBox::operator &= (AABBox const & rhs) AIR_NOEXCEPT;
	template AABBox& AABBox::operator |= (AABBox const & rhs) AIR_NOEXCEPT;

	template AABBox& AABBox::operator = (AABBox const & rhs) AIR_NOEXCEPT;
	template AABBox& AABBox::operator = (AABBox && rhs) AIR_NOEXCEPT;

	template AABBox const AABBox::operator+() const AIR_NOEXCEPT;
	template AABBox const AABBox::operator-() const AIR_NOEXCEPT;

	template float AABBox::getWidth() const AIR_NOEXCEPT;
	template float AABBox::getHeight() const AIR_NOEXCEPT;
	template float AABBox::getDepth() const AIR_NOEXCEPT;

	template float3 const AABBox::getLeftBottomNear() const AIR_NOEXCEPT;
	template float3 const AABBox::getLeftTopNear() const AIR_NOEXCEPT;
	template float3 const AABBox::getRightBottomNear() const AIR_NOEXCEPT;
	template float3 const AABBox::getRightTopNear() const AIR_NOEXCEPT;
	template float3 const AABBox::getLeftBottomFar() const AIR_NOEXCEPT;
	template float3 const AABBox::getLeftTopFar() const AIR_NOEXCEPT;
	template float3 const AABBox::getRightBottomFar() const AIR_NOEXCEPT;
	template float3 const AABBox::getRightTopFar() const AIR_NOEXCEPT;

	template float3& AABBox::getMin() AIR_NOEXCEPT;
	template float3 const & AABBox::getMin() const AIR_NOEXCEPT;

	template float3& AABBox::getMax() AIR_NOEXCEPT;
	template float3 const & AABBox::getMax() const AIR_NOEXCEPT;

	template float3 AABBox::getCenter() const AIR_NOEXCEPT;
	template float3 AABBox::getHalfSize() const AIR_NOEXCEPT;

	template bool AABBox::Intersect(AABBox_T const & aabb) AIR_NOEXCEPT;
	template bool AABBox::Intersect(Frustum const & frustum) AIR_NOEXCEPT;

	template float3 AABBox::getCorner(size_t index) const AIR_NOEXCEPT;


	template bool AABBox::isEmpty() const AIR_NOEXCEPT;
	template float AABBox::getMaxRadiusSq() const AIR_NOEXCEPT;
	template bool AABBox::contains(float3 const & v) const AIR_NOEXCEPT;

	template void AABBox::join(float3 const & point) AIR_NOEXCEPT;

	template void AABBox::join(AABBox const & aabb) AIR_NOEXCEPT;



	template<typename T>
	AABBox_T<T>::AABBox_T() AIR_NOEXCEPT
	{
		mMax = Vector_T<T, 3>::getMinVector();
		mMin = Vector_T<T, 3>::getMaxVector();
	}

	template<typename T>
	AABBox_T<T>::AABBox_T(Vector_T<T, 3> const & vMin, Vector_T<T, 3> const & vMax) AIR_NOEXCEPT
	{
		mMin = vMin;
		mMax = vMax;
	}

	template<typename T>
	AABBox_T<T>::AABBox_T(Vector_T<T, 3> && vMin, Vector_T<T, 3> && vMax) AIR_NOEXCEPT
	{
		mMax = std::move(vMax);
		mMin = std::move(vMin);
	}

	template<typename T>
	AABBox_T<T>::AABBox_T(AABBox_T const & rhs) AIR_NOEXCEPT
	{
		mMax = rhs.mMax;
		mMin = rhs.mMin;
	}

	template<typename T>
	AABBox_T<T>::AABBox_T(AABBox_T && rh) AIR_NOEXCEPT
	{
		mMin = std::move(rh.mMin);
		mMax = std::move(rh.mMax);
	}

	template<typename T>
	bool AABBox_T<T>::isEmpty() const AIR_NOEXCEPT
	{
		return mMax == mMin;
	}

	template<typename T>
	bool AABBox_T<T>::contains(Vector_T<T, 3> const & v) const AIR_NOEXCEPT
	{
		return MathLib::in_bound(v.x(), mMin.x(), mMax.x()) && 
			MathLib::in_bound(v.y(), mMin.y(), mMax.y())&&
			MathLib::in_bound(v.z(), mMin.z(), mMax.z());
	}

	template<typename T>
	float AABBox_T<T>::getMaxRadiusSq() const AIR_NOEXCEPT
	{
		return std::max<float>(mMax.length(), mMin.length());
	}

	

	template<typename T>
	AABBox_T<T>& AABBox_T<T>::operator += (Vector_T<T, 3> const & rhs) AIR_NOEXCEPT
	{
		mMin += rhs;
		mMax += rhs;
		return *this;
	}
	template<typename T>
	AABBox_T<T>& AABBox_T<T>::operator -= (Vector_T<T, 3> const & rhs) AIR_NOEXCEPT
	{
		mMin -= rhs;
		mMax -= rhs;
		return *this;
	}

	template<typename T>
	AABBox_T<T>& AABBox_T<T>::operator *= (float rhs) AIR_NOEXCEPT
	{
		mMin *= rhs;
		mMax *= rhs;
		return *this;
	}

	template<typename T>
	AABBox_T<T>& AABBox_T<T>::operator /= (float rhs)	AIR_NOEXCEPT
	{
		mMin /= rhs;
		mMax /= rhs;
		return *this;
	}

	template<typename T>
	AABBox_T<T>& AABBox_T<T>::operator &= (AABBox_T<T> const & rhs) AIR_NOEXCEPT
	{

 		mMin.x() = std::max<T>(rhs.mMin.x(), mMin.x());
 		mMin.y() = std::max<T>(rhs.mMin.y(), mMin.y());
 		mMin.z() = std::max<T>(rhs.mMin.z(), mMin.z());
 
 
 		mMax.x() = std::min<T>(rhs.mMax.x(), mMax.x());
 		mMax.y() = std::min<T>(rhs.mMax.y(), mMax.y());
 		mMax.z() = std::min<T>(rhs.mMax.z(), mMax.z());
		return *this;
	}
	template<typename T>
	AABBox_T<T>& AABBox_T<T>::operator |= (AABBox_T const & rhs) AIR_NOEXCEPT
	{
		mMin.x() = std::min(rhs.mMin.x(), mMin.x());
		mMin.y() = std::min(rhs.mMin.y(), mMin.y());
		mMin.z() = std::min(rhs.mMin.z(), mMin.z());


		mMax.x() = std::max(rhs.mMax.x(), mMax.x());
		mMax.y() = std::max(rhs.mMax.y(), mMax.y());
		mMax.z() = std::max(rhs.mMax.z(), mMax.z());
		return *this;
	}

	template<typename T>
	AABBox_T<T>& AABBox_T<T>::operator = (AABBox_T const & rhs) AIR_NOEXCEPT
	{
		if (this != &rhs)
		{
			mMax = rhs.mMax;
			mMin = rhs.mMin;
		}
		return *this;
	}
	template<typename T>
	AABBox_T<T>& AABBox_T<T>::operator = (AABBox_T && rhs) AIR_NOEXCEPT
	{
		mMin = std::move(rhs.mMin);
		mMax = std::move(rhs.mMax);
		return *this;
	}
	template<typename T>
	AABBox_T<T> const AABBox_T<T>::operator+() const AIR_NOEXCEPT
	{
		return *this;
	}

	template<typename T>
	AABBox_T<T> const AABBox_T<T>::operator-() const AIR_NOEXCEPT
	{
		return AABBox_T(-this->mMax, -this->mMin);
	}

	template<typename T>
	float AABBox_T<T>::getWidth() const AIR_NOEXCEPT
	{
		return mMax.x() - mMin.x();
	}
	
	template<typename T>
	float AABBox_T<T>::getHeight() const AIR_NOEXCEPT
	{
		return mMax.y() - mMin.y();
	}

	template<typename T>
	float AABBox_T<T>::getDepth() const AIR_NOEXCEPT
	{
		return mMax.z() - mMin.z();
	}

	template<typename T>
	Vector_T<T, 3> const AABBox_T<T>::getLeftBottomNear() const AIR_NOEXCEPT
	{
		return mMin;
	}

	template<typename T>
	Vector_T<T, 3> const AABBox_T<T>::getLeftTopNear() const AIR_NOEXCEPT
	{
		return Vector_T<T, 3>(mMin.x(), mMax.y(), mMin.z());
	}

	template<typename T>
	Vector_T<T, 3> const AABBox_T<T>::getRightBottomNear() const AIR_NOEXCEPT
	{
		return Vector_T<T, 3>(mMax.x(), mMin.y(), mMin.z());
	}

	template<typename T>
	Vector_T<T, 3> const AABBox_T<T>::getRightTopNear() const AIR_NOEXCEPT
	{
		return Vector_T<T, 3>(mMax.x(), mMax.y(), mMin.z());
	}

	template<typename T>
	Vector_T<T, 3> const AABBox_T<T>::getLeftBottomFar() const AIR_NOEXCEPT
	{
		return Vector_T<T, 3>(mMin.x(), mMin.y(), mMax.z());
	}

	template<typename T>
	Vector_T<T, 3> const AABBox_T<T>::getLeftTopFar() const AIR_NOEXCEPT
	{
		return Vector_T<T, 3>(mMin.x(), mMax.y(), mMax.z());
	}

	template<typename T>
	Vector_T<T, 3> const AABBox_T<T>::getRightBottomFar() const AIR_NOEXCEPT
	{
		return Vector_T<T, 3>(mMax.x(), mMin.y(), mMax.z());
	}

	template<typename T>
	Vector_T<T, 3> const AABBox_T<T>::getRightTopFar() const AIR_NOEXCEPT
	{
		return this->mMax;
	}


	template<typename T>
	Vector_T<T, 3>& AABBox_T<T>::getMin() AIR_NOEXCEPT
	{
		return mMin;
	}

	template<typename T>
	Vector_T<T, 3> const & AABBox_T<T>::getMin() const AIR_NOEXCEPT
	{
		return mMin;
	}


	template<typename T>
	Vector_T<T, 3>& AABBox_T<T>::getMax() AIR_NOEXCEPT
	{
		return mMax;
	}

	template<typename T>
	Vector_T<T, 3> const & AABBox_T<T>::getMax() const AIR_NOEXCEPT
	{
		return mMin;
	}

	template<typename T>
	Vector_T<T, 3> AABBox_T<T>::getCenter() const AIR_NOEXCEPT
	{
		return (mMin + mMax) / 2.0f;
	}

	template<typename T>
	Vector_T<T, 3> AABBox_T<T>::getHalfSize() const AIR_NOEXCEPT
	{
		return (mMax - mMin) / 2.0f;
	}

	template<typename T>
	bool AABBox_T<T>::Intersect(AABBox_T const & aabb) AIR_NOEXCEPT
	{
		return MathLib::intersect_aabb_aabb(*this, aabb);
	}

	template<typename T>
	bool AABBox_T<T>::Intersect(Frustum_T<T> const & frustum) AIR_NOEXCEPT
	{
		return MathLib::intersect_aabb_frustum(*this, frustum) != BO_No;
	}

	template<typename T>
	Vector_T<T, 3> AABBox_T<T>::getCorner(size_t index) const AIR_NOEXCEPT
	{
		BOOST_ASSERT(index < 8);
		return Vector_T<T, 3>((index & 1UL) ? mMax.x() : mMin.x(),
			(index & 2UL) ? mMax.y() : mMin.y(),
			(index & 4UL) ? mMax.z() : mMin.z());
	}

	template<typename T>
	void AABBox_T<T>::join(Vector_T<T, 3> const & point) AIR_NOEXCEPT
	{
		mMin = MathLib::minimize(mMin, point);
		mMax = MathLib::maximize(mMax, point);
	}


	template<typename T>
	void AABBox_T<T>::join(AABBox_T<T> const & aabb) AIR_NOEXCEPT
	{
		mMin = MathLib::minimize(mMin, aabb.getMin());
		mMax = MathLib::maximize(mMax, aabb.getMax());
	}
}