
#include "PreDeclare.h"
#include "Context.h"
#include "Engine.h"
#include "SingletonManager.hpp"
#include "SampleCommon.hpp"
#include "core/include/Engine.h"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/RenderEngine.hpp"
#include "rendersystem/include/RenderView.hpp"
#include "scene_manager/include/SceneObjectHelper.hpp"
#include "scene_manager/include/SceneObject.hpp"
#include "rendersystem/include/SimpleMeshFactory.hpp"
#include "rendersystem/include/Mesh.hpp"
#include "random"

#include "sample/include/RenderOneBall.hpp"

using namespace Air;
namespace
{

	enum
	{
		Exit,
	};
}

namespace Sample
{
	SampleRenderOneBall::SampleRenderOneBall() : App3DFramework("createWindow")
	{

	}

	void SampleRenderOneBall::onCreate()
	{
		mSphere = MakeSharedPtr<SphereObject>(float4(0.0147f, 0.0332f, 0.064f, 1.0f), float4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
		mSphere->setLocalMatrix(MathLib::scaling(1.3f, 1.3f, 1.3f) * MathLib::translation(0.0f, 0.0f, 0.0f));
		mSphere->addToSceneManager();

		this->LookAt(float3(20.0f, 0.f, 0.0f), float3(0.0f, 0.0f, 0.0f));
		this->Proj(0.05f, 100.0f);


		mCameraController.attachCamera(this->getActiveCamera());
		mCameraController.setScalers(0.01f, 0.05f);
	}

	uint32_t SampleRenderOneBall::doUpdate(uint32_t pass)
	{
		RenderEngine& re = SingletonManager::getRenderFactoryInstance().getRenderEngineInstance();
		Color c(0.0, 0.0, 0.0, 1.0);
		re.getCurrentFrameBuffer()->clear(FrameBuffer::CBM_Color | FrameBuffer::CBM_Depth | FrameBuffer::CBM_Stencil, c, 1.0, 0);
		return App3DFramework::URV_NeedFlush | App3DFramework::URV_Finished;
	}


}

int SampleMain()
{
	ContextCfg  cfg = Engine::getInstance().getConfig();
	Sample::SampleRenderOneBall app;
	app.create();
	app.run();
	return 0;
}

