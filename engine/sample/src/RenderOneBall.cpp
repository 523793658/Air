#include "PreDeclare.h"
#include "core/include/Engine.h"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/RenderEngine.hpp"
#include "rendersystem/include/RenderView.hpp"
#include "scene_manager/include/SceneObjectHelper.hpp"
#include "rendersystem/include/SimpleMeshFactory.hpp"
#include "sample/include/RenderOneBall.hpp"
#include "rendersystem/include/Mesh.hpp"
#include "random"
using namespace Air;

namespace
{
	class SphereObject : public SceneObjectHelper
	{
	public:
		SphereObject(float4 const & diff, float4 const &spec, float glossiness)
			:SceneObjectHelper(SOA_Cullable)
		{
			mRenderable = SimpleMeshFactory::createStaticShpere(1.0);
		}

		void setRenderingType(int type)
		{

		}

	};

	enum
	{
		Exit,
	};



}

SampleRenderOneBall::SampleRenderOneBall() : App3DFramework("createWindow")
{

}

int SampleMain()
{
	ContextCfg  cfg = Engine::getInstance().getConfig();
	SampleRenderOneBall app;
	app.create();
	app.run();
	return 0;
}

void SampleRenderOneBall::onCreate()
{
	mSphere = MakeSharedPtr<SphereObject>(float4(0.0147f, 0.0332f, 0.064f, 1.0f), float4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
	mSphere->setLocalMatrix(MathLib::scaling(1.3f, 1.3f, 1.3f) * MathLib::translation(0.0f, 0.0f, 0.0f));
	mSphere->addToSceneManager();

	this->LookAt(float3(0.0f, 0.2f, -0.6f), float3(0.0f, 0.0f, 0.0f));
	this->Proj(0.05f, 100.0f);



}

uint32_t SampleRenderOneBall::doUpdate(uint32_t pass)
{
	RenderEngine& re = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
	Color c(0.0, 0.0, 0.0, 1.0 );
	re.getCurrentFrameBuffer()->clear(FrameBuffer::CBM_Color | FrameBuffer::CBM_Depth | FrameBuffer::CBM_Stencil, c, 1.0, 0);
	return App3DFramework::URV_NeedFlush | App3DFramework::URV_Finished;
}
