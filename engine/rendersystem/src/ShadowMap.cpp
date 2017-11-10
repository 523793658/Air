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
		Frustum const * frustum = &srcCamera->getViewFrustum();
		dstCamera->setViewParams(float3(0, 0, 0), dir);
		float4x4 const &viewMatrix = dstCamera->getViewMatrix();
		std::vector<float3> vertex;
		std::vector<uint16_t> indices;
		factor2 = factor2 * factor2;
		factor = factor * factor;
		for (int i = 0; i < 4; i++)
		{
			aabb.join(MathLib::transform_coord(MathLib::vectorLerp(frustum->getCorner(i), frustum->getCorner(i + 4), factor2), viewMatrix));
			aabb.join(MathLib::transform_coord(MathLib::vectorLerp(frustum->getCorner(i), frustum->getCorner(i + 4), factor), viewMatrix));
		}
		float3 eye = aabb.getCenter();
		eye.z() -= aabb.getDepth() / 2.0f;
		eye = MathLib::transform_coord(eye, dstCamera->getInverseViewMatirx());

		dstCamera->setViewParams(eye, eye + dir);
		dstCamera->setProjOrthoParams(aabb.getWidth(), aabb.getHeight(), 0, aabb.getDepth());
	}



}