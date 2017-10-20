#include "Engine.h"
#include "Context.h"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderEngine.hpp"
#include "Camera.hpp"

namespace Air
{
	enum CameraMode
	{
		CM_Jitter = 1UL << 0,
		CM_Omni = 1UL << 1
	};

	Camera::Camera()
		:mViewProjMatDirty(true), mViewProjMatrixWoAdjustDirty(true), mFrustumDirty(true), mMode(0), mCurJitterIndex(0)
	{
		RenderEngine& re = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		uint32_t num_motion_frames = re.getNumMotionFrames();
		mPrevProjMats.resize(num_motion_frames);
		mPrevProjMats.resize(num_motion_frames);
		this->setViewParams(float3(0, 0, 0), float3(0, 0, 1), float3(0, 1, 0));
		this->setProjParams(PI / 4, 1, 1, 1000);
	}

	void Camera::setViewParams(float3 const & eye_pos, float3 const & look_at)
	{
		this->setViewParams(eye_pos, look_at, float3(0.0f, 1.0f, 0.0f));
	}
	void Camera::setViewParams(float3 const & eye_pos, float3 const & look_at, float3 const & up_vec)
	{
		mLookDistance = (look_at - eye_pos).length();
		mViewMatrix = float4x4::createLookAtLH(eye_pos, look_at, up_vec);
		mInvViewMatrix = mViewMatrix.inverse();
		mViewProjMatDirty = true;
		mViewProjMatrixWoAdjustDirty = true;
		mFrustumDirty = true;
	}

	void Camera::setProjParams(float fov, float aspect, float near_plane, float far_plane)
	{
		mFov = fov;
		mAspect = aspect;
		mNearPlane = near_plane;
		mFarPlane = far_plane;

		mProjectMatrix = float4x4::createPerspectiveFOVLH(fov, aspect, near_plane, far_plane);
		mProjectMatrixWoAdjust = mProjectMatrix;

		RenderEngine& re = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		re.adjustProjectionMatrix(mProjectMatrix);
		mInvProjectMatrix = mProjectMatrix.inverse();
		mInvProjectMatrixWoAdjust = mProjectMatrixWoAdjust.inverse();
		mViewProjMatDirty = true;
		mViewProjMatrixWoAdjustDirty = true;
		mFrustumDirty = true;

	}
	void Camera::setProjOrthoParams(float w, float h, float near_plane, float far_plane)
	{
		mFov = 0.0f;
		mAspect = w / h;
		mNearPlane = near_plane;
		mFarPlane = far_plane;

		mProjectMatrix = float4x4::createOrthoLH(w, h, near_plane, far_plane);
		mProjectMatrixWoAdjust = mProjectMatrix;
		RenderEngine& re = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		re.adjustProjectionMatrix(mProjectMatrix);
		mInvProjectMatrix = mProjectMatrix.inverse();
		mInvProjectMatrixWoAdjust = mProjectMatrixWoAdjust.inverse();
		mViewProjMatDirty = true;
		mViewProjMatrixWoAdjustDirty = true;
		mFrustumDirty = true;
	}
	void Camera::setProjOrthoParams(float left, float top, float right, float bottom, float near_plane, float far_plane)
	{
		mFov = 0.0f;
		mAspect = (right - left) / (top - bottom);
		mNearPlane = near_plane;
		mFarPlane = far_plane;

		mProjectMatrix = float4x4::createOrthoCenterLH(left, right, bottom, top, near_plane, far_plane);
		mProjectMatrixWoAdjust = mProjectMatrix;

		RenderEngine& re = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		re.adjustProjectionMatrix(mProjectMatrix);
		mInvProjectMatrix = mProjectMatrix.inverse();
		mInvProjectMatrixWoAdjust = mProjectMatrixWoAdjust.inverse();

		mViewProjMatDirty = true;
		mViewProjMatrixWoAdjustDirty = true;
		mFrustumDirty = true;
	}


	Frustum const & Camera::getViewFrustum() const
	{
		if (mFrustumDirty)
		{
			mFrustum.clipMatrix(getViewProjMatrixWOAdjust(), getInverseViewProjMatrixWOAdjust());
			BoundOverlap bo = mFrustum.intersect(AABBox(float3(-5.0f, -0.5f, 4.0f), float3(-4.0f, 0.5f, 5.0f)));
			mFrustumDirty = false;
		}
		return mFrustum;
	}

	bool Camera::getOmniDirectionalMode() const
	{
		return (mMode & CM_Omni) > 0;
	}

	void Camera::setOmniDirectionalMode(bool omni)
	{
		if (omni)
		{
			mMode |= CM_Omni;
		}
		else
		{
			mMode &= ~CM_Omni;
		}
	}

	bool Camera::getJitterMode() const
	{
		return (mMode & CM_Jitter) > 0;
	}
	void Camera::setJitterMode(bool jitter)
	{
		if (jitter)
		{
			mMode |= CM_Jitter;
		}
		else
		{
			mMode &= ~CM_Jitter;
		}
	}

	float4x4 const & Camera::getViewMatrix() const
	{
		return mViewMatrix;
	}
	float4x4 const & Camera::getProjMatrix() const
	{
		return mProjectMatrix;
	}
	float4x4 const & Camera::getProjMatrixWOAdjust() const
	{
		return mProjectMatrixWoAdjust;
	}
	float4x4 const & Camera::getViewProjMatrix() const
	{
		if (mViewProjMatDirty)
		{
			mViewProjectMatrix = mViewMatrix * mProjectMatrix;
			mInvViewPorjectMatirx = mInvProjectMatrix * mInvViewMatrix;
			mViewProjMatDirty = false;
		}
		return mViewProjectMatrix;
	}
	float4x4 const & Camera::getViewProjMatrixWOAdjust() const
	{
		if (mViewProjMatrixWoAdjustDirty)
		{
			mViewProjectMatrixWOAdjust = mViewMatrix * mProjectMatrixWoAdjust;
			mInvViewProjectMatrixWOAdjust = mInvProjectMatrixWoAdjust * mInvViewMatrix;
			mViewProjMatrixWoAdjustDirty = false;
		}
		return mViewProjectMatrixWOAdjust;

	}

	float4x4 const & Camera::getInverseViewProjMatrixWOAdjust() const
	{
		if (mViewProjMatrixWoAdjustDirty)
		{
			mViewProjectMatrixWOAdjust = mViewMatrix * mProjectMatrixWoAdjust;
			mInvViewProjectMatrixWOAdjust = mInvProjectMatrixWoAdjust * mInvViewMatrix;
			mViewProjMatrixWoAdjustDirty = false;
		}
		return mInvViewProjectMatrixWOAdjust;
	}
}