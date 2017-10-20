#ifndef _Core_Camera_H_
#define _Core_Camera_H_
#pragma once
#include <boost/circular_buffer.hpp>
#include "basic/include/Frustum.hpp"
#include "basic/include/Vector.hpp"
#include "basic/include/Matrix.hpp"
#include "PreDeclare.h"
namespace Air
{
	class AIR_CORE_API Camera : public std::enable_shared_from_this<Camera>
	{
	public:
		Camera();
		float3 const & getEyePos() const
		{
			return *reinterpret_cast<float3 const *>(&mInvViewMatrix.row(3));
		}

		float3 getLookAt() const
		{
			return this->getEyePos() + this->getForwardVec()* mLookDistance;
		}

		float3 getRightVec() const
		{
			return *reinterpret_cast<float3 const *>(&mInvViewMatrix.row(0));
		}

		float3 getUpVec() const
		{
			return *reinterpret_cast<float3 const *>(&mInvViewMatrix.row(1));
		}

		float3 const & getForwardVec() const
		{
			return *reinterpret_cast<float3 const *>(&mInvViewMatrix.row(2));
		}

		float getLookAtDist() const
		{
			return mLookDistance;
		}

		void setViewParams(float3 const & eye_pos, float3 const & look_at);
		void setViewParams(float3 const & eye_pos, float3 const & look_at, float3 const & up_vec);
		void setProjParams(float fov, float aspect, float near_plane, float far_plane);
		void setProjOrthoParams(float w, float h, float near_plane, float far_plane);
		void setProjOrthoParams(float left, float top, float right, float bottom, float near_plane, float far_plane);
		Frustum const & getViewFrustum() const;

		float4x4 const & getViewMatrix() const;
		float4x4 const & getProjMatrix() const;
		float4x4 const & getProjMatrixWOAdjust() const;
		float4x4 const & getViewProjMatrix() const;
		float4x4 const & getViewProjMatrixWOAdjust() const;
		float4x4 const & getInverseViewProjMatrixWOAdjust() const;

		bool getOmniDirectionalMode() const;

		void setOmniDirectionalMode(bool omni);

		bool getJitterMode() const;
		void setJitterMode(bool jitter);

		float getFarPlane() const
		{
			return mFarPlane;
		}

	private:
		float mLookDistance;
		float4x4 mViewMatrix;
		float4x4 mInvViewMatrix;
		mutable Frustum mFrustum;

		float mFov;
		float mAspect;
		float mNearPlane;
		float mFarPlane;
		float4x4 mProjectMatrix;
		float4x4 mInvProjectMatrix;

		float4x4 mProjectMatrixWoAdjust;
		float4x4 mInvProjectMatrixWoAdjust;

		mutable float4x4 mViewProjectMatrix;
		mutable float4x4 mInvViewPorjectMatirx;

		mutable float4x4 mViewProjectMatrixWOAdjust;
		mutable float4x4 mInvViewProjectMatrixWOAdjust;

		boost::circular_buffer<float4x4> mPrevViewMats;
		boost::circular_buffer<float4x4> mPrevProjMats;

		mutable bool mViewProjMatDirty;
		mutable bool mViewProjMatrixWoAdjustDirty;
		mutable bool mFrustumDirty;

		uint32_t mMode;
		int mCurJitterIndex;


	};
}


#endif