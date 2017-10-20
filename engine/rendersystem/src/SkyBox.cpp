#include "Engine.h"
#include "basic/include/Math.hpp"
#include "rendersystem/include/GraphicsBuffer.hpp"
#include "rendersystem/include/RenderEngine.hpp"
#include "rendersystem/include/RenderEffect.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "Camera.hpp"
#include "app/include/App3D.hpp"
#include "rendersystem/include/SkyBox.hpp"
namespace Air

{
	RenderableSkyBox::RenderableSkyBox()
		:RenderableHelper(L"SkyBox")
	{
		RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();

		RenderEffectPtr effect = syncLoadRenderEffect("assets/shader/skyBox.asd");

		this->setTechnique(effect, effect->getTechniqueByName("SkyBoxTech"));

		float3 xyzs[] =
		{
			float3(1.0f, 1.0f, 1.0f),
			float3(1.0f, -1.0f, 1.0f),
			float3(-1.0f, 1.0f, 1.0f),
			float3(-1.0f, -1.0f, 1.0f)
		};

		mRenderlayout = rf.MakeRenderLayout();
		mRenderlayout->setTopologyType(RenderLayout::TT_TriangleStrip);

		GraphicsBufferPtr vb = rf.makeVertexBuffer(BU_Static, EAH_GPU_Read | EAH_Immutable, sizeof(xyzs), xyzs);

		mRenderlayout->bindVertexStream(vb, VertexElement(VEU_Position, 0, EF_BGR32F));

		mAABB = MathLib::compute_aabbox(&xyzs[0], &xyzs[4]);
		mTCAABB = AABBox(float3(0, 0, 0), float3(0, 0, 0));
	}
	
	void RenderableSkyBox::setTechnique(RenderEffectPtr const & effect, RenderTechnique* tech)
	{
		mEffect = effect;
		mTechnique = tech;

		mSkyCubeTex = mEffect->getParameterByName("u_SkyBoxTex");
		mSkyCcubeTex = mEffect->getParameterByName("u_SkyBoxCcubeTex");
		mSkyCompressed = mEffect->getParameterByName("u_SkyBoxCompressed");
		mDepthFar = mEffect->getParameterByName("u_DepthFar");
		mInvMVP = mEffect->getParameterByName("u_InvMVP");
	}


	void RenderableSkyBox::setCubeMap(TexturePtr const & cube)
	{
		*mSkyCubeTex = cube;
		*mSkyCompressed = static_cast<int32_t>(0);
	}
	void RenderableSkyBox::setCompressedCubeMap(TexturePtr const & y_cube, TexturePtr const & c_cube)
	{
		*mSkyCubeTex = y_cube;
		*mSkyCcubeTex = c_cube;
		*mSkyCompressed = static_cast<int32_t>(1);
	}

	void RenderableSkyBox::setPass(PassType type)
	{
		switch (type)
		{
		case Air::PT_OpaqueGBufferMRT:
			break;
		case Air::PT_OpaqueSpecialShading:
			break;
		default:
			break;
		}
	}

	void RenderableSkyBox::onRenderBegin()
	{
		App3DFramework const & app = Engine::getInstance().getAppInstance();
		Camera const & camera = app.getActiveCamera();
		*mDepthFar = camera.getFarPlane();
		float4x4 rot_view = camera.getViewMatrix();
		rot_view(3, 0) = 0;
		rot_view(3, 1) = 0;
		rot_view(3, 2) = 0;
		*mInvMVP = MathLib::inverse(rot_view * camera.getProjMatrix());
	}
}