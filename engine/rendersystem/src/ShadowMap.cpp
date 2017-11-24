#include "Context.h"
#include "Camera.hpp"
#include "rendersystem/include/RenderableHelper.hpp"
#include "scene_manager/include/SceneObject.hpp"
#include "rendersystem/include/ShadowMap.hpp"

namespace Air
{
	void ShadowUtil::adjustShadowCamera(Camera* srcCamera, Camera* dstCamera, float3 const & dir, float factor, float factor2)
	{
		AABBox aabb;
		dstCamera->setViewParams(float3(0, 0, 0), dir);
		float4x4 const &light_view_matrix = srcCamera->getInverseViewProjMatrixWOAdjust() * dstCamera->getViewMatrix();
		float4x4 const & projMat = srcCamera->getProjMatrixWOAdjust();
		float d1 = MathLib::transform_coord(float3(0.0f, 0.0f, factor), projMat).z();
		float d2 = MathLib::transform_coord(float3(0.0f, 0.0f, factor2), projMat).z();
	
		aabb.join(MathLib::transform_coord(float3(-1, -1, d1), light_view_matrix));
		aabb.join(MathLib::transform_coord(float3(1, -1, d1), light_view_matrix));
		aabb.join(MathLib::transform_coord(float3(-1, 1, d1), light_view_matrix));
		aabb.join(MathLib::transform_coord(float3(1, 1, d1), light_view_matrix));

		aabb.join(MathLib::transform_coord(float3(-1, -1, d2), light_view_matrix));
		aabb.join(MathLib::transform_coord(float3(1, -1, d2), light_view_matrix));
		aabb.join(MathLib::transform_coord(float3(-1, 1, d2), light_view_matrix));
		aabb.join(MathLib::transform_coord(float3(1, 1, d2), light_view_matrix));

		float3 eye = aabb.getCenter();
		eye.z() -= aabb.getDepth() / 2.0f;
		eye = MathLib::transform_coord(eye, dstCamera->getInverseViewMatirx());

		dstCamera->setViewParams(eye, eye + dir);
		dstCamera->setProjOrthoParams(aabb.getWidth(), aabb.getHeight(), 0, aabb.getDepth() + 200);
	}

	AABBox ShadowUtil::calcFrustumExtents(Camera const & camera, float nearPlane, float farPlane, float4x4 const & light_view_proj)
	{
		float const inv_scale_x = 1.0f / camera.getProjMatrix()(0, 0);
		float const inv_scale_y = 1.0f / camera.getProjMatrix()(1, 1);
		float4x4 const view_to_light_proj = camera.getInverseViewMatirx() * light_view_proj;

		float3 corners[8];
		float near_x = inv_scale_x * nearPlane;
		float near_y = inv_scale_y * nearPlane;
		corners[0] = float3(-near_x, near_y, nearPlane);
		corners[1] = float3(-near_x, near_y, nearPlane);
		corners[2] = float3(-near_x, -near_y, nearPlane);
		corners[3] = float3(near_x, -near_y, nearPlane);

		float far_x = inv_scale_x * farPlane;
		float far_y = inv_scale_y * farPlane;

		corners[4] = float3(-far_x, far_y, farPlane);
		corners[5] = float3(far_x, far_y, farPlane);
		corners[6] = float3(-far_x, -far_y, farPlane);
		corners[7] = float3(far_x, -far_y, farPlane);

		for (int i = 0; i < 8; ++i)
		{
			corners[i] = MathLib::transform_coord(corners[i], light_view_proj);
		}
		return MathLib::compute_aabbox(corners, corners + 8);
	}



}