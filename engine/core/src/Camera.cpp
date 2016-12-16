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

	void Camera::setViewParams(float3 const & eye_pos, float3 const & look_at)
	{
		this->setViewParams(eye_pos, look_at, float3(0.0f, 1.0f, 0.0f));
	}
	void Camera::setViewParams(float3 const & eye_pos, float3 const & look_at, float3 const & up_vec)
	{
		mLookDistance = (look_at - eye_pos).Length();
		mViewMatrix = float4x4::CreateLookAt(eye_pos, look_at, up_vec);
		mInvViewMatrix = mViewMatrix.Invert();
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

		mProjectMatrix = float4x4::CreatePerspectiveFieldOfView(fov, aspect, near_plane, far_plane);
		mProjectMatrixWoAdjust = mProjectMatrix;

		RenderEngine& re = Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		re.adjustProjectionMatrix(mProjectMatrix);
		mInvProjectMatrix = mProjectMatrix.Invert();
		mInvProjectMatrixWoAdjust = mProjectMatrixWoAdjust.Invert();
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

		mProjectMatrix = float4x4::CreateOrthographic(w, h, near_plane, far_plane);
		mProjectMatrixWoAdjust = mProjectMatrix;
		RenderEngine& re = Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		re.adjustProjectionMatrix(mProjectMatrix);
		mInvProjectMatrix = mProjectMatrix.Invert();
		mInvProjectMatrixWoAdjust = mProjectMatrixWoAdjust.Invert();
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

		mProjectMatrix = float4x4::CreateOrthographicOffCenter(left, right, bottom, top, near_plane, far_plane);
		mProjectMatrixWoAdjust = mProjectMatrix;

		RenderEngine& re = Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		re.adjustProjectionMatrix(mProjectMatrix);
		mInvProjectMatrix = mProjectMatrix.Invert();
		mInvProjectMatrixWoAdjust = mProjectMatrixWoAdjust.Invert();

		mViewProjMatDirty = true;
		mViewProjMatrixWoAdjustDirty = true;
		mFrustumDirty = true;
	}


	Frustum const & Camera::getViewFrustum() const
	{
		if (mFrustumDirty)
		{
			Frustum::CreateFromMatrix(mFrustum, mProjectMatrixWoAdjust);
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
	float4x4 const & Camera::getViewProjMatrix()
	{
		if (mViewProjMatDirty)
		{
			mViewProjectMatrix = mViewMatrix * mProjectMatrix;
			mInvViewPorjectMatirx = mInvProjectMatrix * mInvViewMatrix;
			mViewProjMatDirty = false;
		}
		return mViewProjectMatrix;
	}
	float4x4 const & Camera::getViewProjMatrixWOAdjust()
	{
		if (mViewProjMatrixWoAdjustDirty)
		{
			mViewProjectMatrixWOAdjust = mViewMatrix * mProjectMatrixWoAdjust;
			mInvViewProjectMatrixWOAdjust = mInvProjectMatrixWoAdjust * mInvViewMatrix;
			mViewProjMatrixWoAdjustDirty = false;
		}
		return mViewProjectMatrixWOAdjust;

	}

}