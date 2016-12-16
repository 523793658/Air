#include "core/include/Engine.h"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/RenderEngine.hpp"
#include "rendersystem/include/RenderView.hpp"
#include "scene_manager/include/SceneObjectHelper.hpp"
#include "sample/include/RenderOneBall.hpp"

using namespace Air;

namespace
{
	class SphereObject : public SceneObjectHelper
	{
	public:
		SphereObject(float4 const & diff, float4 const &spec, float glossiness)
			:SceneObjectHelper(SOA_Cullable)
		{
			//º”‘ÿrenderableObj
			mRenderable = nullptr;
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
	ContextCfg  cfg = Context::getInstance().getConfig();
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
	RenderEngine& re = Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
	Color32 c = { 1.0, 1.0, 0.0, 1.0 };
	re.getCurrentFrameBuffer()->clear(FrameBuffer::CBM_Color | FrameBuffer::CBM_Depth | FrameBuffer::CBM_Stencil, c, 1.0, 0);
	return App3DFramework::URV_NeedFlush | App3DFramework::URV_Finished;
}
