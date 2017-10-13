#include "Engine.h"
#include "SampleCommon.hpp"
#include "CameraController.hpp"
#include "input_system/include/InputFactory.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/Light.hpp"
#include "app/include/App3D.hpp"


using namespace Air;
namespace Sample
{
	enum
	{
		Exit,
	};

	InputActionDefine actions[] =
	{
		InputActionDefine(Exit, KS_Escape),
	};


	class SimpleLighting : public App3DFramework
	{
	public:
		SimpleLighting() : App3DFramework("Simple Lighting")
		{

		}
	private:

		uint32_t doUpdate(uint32_t pass)
		{
			RenderEngine& re = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
			re.getCurrentFrameBuffer()->clear(FrameBuffer::CBM_Color | FrameBuffer::CBM_Depth | FrameBuffer::CBM_Stencil, Color(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);
			return App3DFramework::URV_NeedFlush | App3DFramework::URV_Finished;
		}
		void onCreate()
		{

			AmbientLightSourcePtr ambient_light = MakeSharedPtr<AmbientLightSource>();

			ambient_light->addToSceneManager();


			uint32_t spheres_row = 10;
			uint32_t spheres_column = 10;
			mSpheres.resize(spheres_row * spheres_column);
			for (uint32_t i = 0; i < spheres_row; ++i)
			{
				for (uint32_t j = 0; j < spheres_column; ++j)
				{
					mSpheres[i * spheres_column + j] = MakeSharedPtr<SphereObject>(float4(1.0f, 1.0f, 1.0f, 1.0f), float4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
					mSpheres[i * spheres_column + j]->setLocalMatrix(MathLib::scaling(1.3f, 1.3f, 1.3f) * MathLib::translation((-static_cast<float>(spheres_column / 2) + j + 0.5f),
						0.0f,
						(-static_cast<float>(spheres_row / 2) + i + 0.5f)));
					mSpheres[i * spheres_column + j]->addToSceneManager();
				}
			}

			this->LookAt(float3(10.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f));
			this->Proj(0.05f, 100);
			mCameraController.attachCamera(this->getActiveCamera());
			mCameraController.setScalers(0.003f, 0.003f);

			InputEngine& inputEngine = Engine::getInstance().getInputFactoryInstance().getInputEngineInstance();
			InputActionMap actionMap;
			actionMap.addActions(actions, actions + std::size(actions));
			ActionHandlerT input_handler = MakeSharedPtr<InputSignal>();
			input_handler->connect(std::bind(&SimpleLighting::inputHandler, this, std::placeholders::_1, std::placeholders::_2));
			inputEngine.actionMap(actionMap, input_handler);


		}

		void inputHandler(Air::InputEngine const & sender, Air::InputAction const & action)
		{

		}
	private:
		std::vector<Air::SceneObjectPtr> mSpheres;
		Air::TrackballCameraController mCameraController;
	};
}
int SampleMain()
{
	ContextCfg  cfg = Engine::getInstance().getConfig();
	Sample::SimpleLighting app;
	app.create();
	app.run();
	return 0;
}