#include <core/include/Engine.h>
#include "rendersystem/include/SimpleMeshFactory.hpp"
#include "rendersystem/include/Renderable.hpp"
#include "rendersystem/include/RenderableHelper.hpp"
#include "rendersystem/include/Mesh.hpp"
#include "rendersystem/include/RenderMaterial.hpp"
#include "SampleCommon.hpp"

int main()
{
	return entryFunc();
}

namespace Sample
{
	SphereObject::SphereObject(float3 const & diff, float3 const &spec, float glossiness, float metallic)
		:SceneObjectHelper(SOA_Cullable)
	{
		mRenderable = SimpleMeshFactory::createStaticShpere(0.5);
		mRenderable->setMaterial(MakeSharedPtr<RenderMaterial>(diff, spec, glossiness, metallic));
	}

	CubeObject::CubeObject(float4 const & diff, float4 const & spec, float glossiness): SceneObjectHelper(SOA_Cullable)
	{
		mRenderable = SimpleMeshFactory::createStaticCube();
	}

}