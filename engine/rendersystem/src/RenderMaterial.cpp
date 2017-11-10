#include "Context.h"
#include "rendersystem/include/RenderMaterial.hpp"
namespace Air
{
	RenderMaterial::RenderMaterial()
	{

	}
	RenderMaterial::RenderMaterial(float3 const & diff, float3 const &spec, float glossiness, float metallic)
	{
		mSpecularMetalness.x() = spec.x();
		mSpecularMetalness.y() = spec.y();
		mSpecularMetalness.z() = spec.z();
		mSpecularMetalness.w() = metallic;
		mAlbedoRoughness.x() = diff.x();
		mAlbedoRoughness.y() = diff.y();
		mAlbedoRoughness.z() = diff.z();
		mAlbedoRoughness.w() = glossiness;
	}
}
