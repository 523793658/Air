#ifndef _Core_Camera_H_
#define _Core_Camera_H_
#pragma once
#include <boost/circular_buffer.hpp>
#include "basic/include/Frustum.hpp"

namespace Air
{
	class AIR_CORE_API Camera : public std::enable_shared_from_this<Camera>
	{
	public:
		void setViewParams(float3 const & eye_pos, float3 const & look_at);
		void setViewParams(float3 const & eye_pos, float3 const & look_at, float3 const & up_vec);
		void setProjParams(float fov, float aspect, float near_plane, float far_plane);
		void setProjOrthoParams(float w, float h, float near_plane, float far_plane);
		void setProjOrthoParams(float left, float top, float right, float bottom, float near_plane, float far_plane);
		Frustum const & getViewFrustum() const;

		float4x4 const & getViewMatrix() const;
		float4x4 const & getProjMatrix() const;
		float4x4 const & getProjMatrixWOAdjust() const;
		float4x4 const & getViewProjMatrix();
		float4x4 const & getViewProjMatrixWOAdjust();

		bool getOmniDirectionalMode() const;

		void setOmniDirectionalMode(bool omni);

		bool getJitterMode() const;
		void setJitterMode(bool jitter);

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

		float4x4 mViewProjectMatrix;
		float4x4 mInvViewPorjectMatirx;

		float4x4 mViewProjectMatrixWOAdjust;
		float4x4 mInvViewProjectMatrixWOAdjust;


		mutable bool mViewProjMatDirty;
		mutable bool mViewProjMatrixWoAdjustDirty;
		mutable bool mFrustumDirty;

		uint32_t mMode;


	};
}


#endif