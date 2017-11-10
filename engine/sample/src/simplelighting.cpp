#include "Context.h"
#include "Engine.h"
#include "SingletonManager.hpp"
#include "SampleCommon.hpp"
#include "CameraController.hpp"
#include "input_system/include/InputFactory.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/Light.hpp"
#include "app/include/App3D.hpp"
#include "rendersystem/include/RenderMaterial.hpp"
#include "rendersystem/include/Mesh.hpp"
#include "rendersystem/include/SimpleMeshFactory.hpp"
#include "rendersystem/include/RenderableHelper.hpp"
#include "rendersystem/include/RenderEffect.hpp"
#include "rendersystem/include/Renderable.hpp"


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

	uint32_t reverseBits32(uint32_t bits)
	{
		bits = (bits << 16) | (bits >> 16);
		bits = ((bits & 0x00ff00ff) << 8) | ((bits & 0xff00ff00) >> 8);
		bits = ((bits & 0x0f0f0f0f) << 4) | ((bits & 0xf0f0f0f0) >> 4);
		bits = ((bits & 0x33333333) << 2) | ((bits & 0xcccccccc) >> 2);
		bits = ((bits & 0x55555555) << 1) | ((bits & 0xaaaaaaaa) >> 1);
		return bits;
	}

	float2 hammersley(uint32_t index, uint32_t numSamples, uint2 random)
	{
		float E1 = Air::MathLib::frac((float)index / numSamples + ((float)(random.x() & 0xffff) / (1 << 16)));
		float E2 = float(reverseBits32(index) ^ random.y()) * 2.3283064365386963e-10f;
		return float2(E1, E2);
	}


	float2 IntegrateBRDFBP(float shininess, float n_dot_v)
	{
		float3 view(sqrt(1.0f - n_dot_v * n_dot_v), 0, n_dot_v);
		float2 rg(0, 0);

		uint32_t const NUM_SAMPLES = 1024;
		for (uint32_t i = 0; i < NUM_SAMPLES; ++i)
		{
			
		}

		return rg / static_cast<float>(NUM_SAMPLES);
	}






	class SimpleLighting : public App3DFramework
	{
	public:
		SimpleLighting() : App3DFramework("Simple Lighting")
		{

		}
	private:

		uint32_t doUpdate(uint32_t pass)
		{
			return App3DFramework::URV_NeedFlush | App3DFramework::URV_Finished;
		}
		void onCreate()
		{
			TexturePtr y_cube_map = aSyncLoadTexture("assets/texture/uffizi_cross_filtered_y.dds", EAH_GPU_Read | EAH_Immutable);
			

			TexturePtr c_cube_map = aSyncLoadTexture("assets/texture/uffizi_cross_filtered_c.dds", EAH_GPU_Read | EAH_Immutable);



			TexturePtr integratedBRDFTex = aSyncLoadTexture("assets/texture/integratedBRDF.dds", EAH_GPU_Read | EAH_Immutable);

			TexturePtr sky_box_tex = aSyncLoadTexture("assets/texture/uffizi_cross.dds", EAH_GPU_Read | EAH_Immutable);

			AmbientLightSourcePtr ambient_light = MakeSharedPtr<AmbientLightSource>();
			SceneObjectPtr ambientLightObject = MakeSharedPtr<SceneObject>(SceneObject::SOA_LightSource);
			ambient_light->setColor(float3(0.3, 0.3, 0.3) * 0.8f);
			ambient_light->setSkyLightTexC(c_cube_map);
			ambient_light->setSkyLightTexY(y_cube_map);
			ambientLightObject->setCustomData(ambient_light);
			ambientLightObject->addToSceneManager();


			DirectLightSourcePtr direction_light = MakeSharedPtr<DirectLightSource>();
			direction_light->setColor(float3(1, 1, 1) * 0.8f);
			direction_light->setDirection(float3(1, 1, 1));
			SceneObjectPtr directLightObject = MakeSharedPtr<SceneObject>(SceneObject::SOA_LightSource);
			directLightObject->setCustomData(direction_light);
			directLightObject->addToSceneManager();
			uint32_t spheres_row = 10;
			uint32_t spheres_column = 10;
			mSpheres.resize(spheres_row * spheres_column);
			for (uint32_t i = 0; i < spheres_row; ++i)
			{
				for (uint32_t j = 0; j < spheres_column; ++j)
				{
					//½ðÊôÐÔXÖá
					//´Ö²Ú¶ÈZÖá
					mSpheres[i * spheres_column + j] = MakeSharedPtr<SphereObject>(float3(1.0f, 1.0f, 1.0f), float3(1.0f, 1.0f, 1.0f), static_cast<float>(i)/ (spheres_row-1.0f), static_cast<float>(j) / (spheres_column - 1.0f));
					mSpheres[i * spheres_column + j]->setLocalMatrix(MathLib::translation((-static_cast<float>(spheres_column / 2) + j + 0.5f),
						0.0f,
						(-static_cast<float>(spheres_row / 2) + i + 0.5f)));
					RenderEffectPtr const & effect = mSpheres[i * spheres_column + j]->getRenderable()->getRenderEffect();
					*effect->getParameterByName("u_SkyBoxTex") = y_cube_map;
					*effect->getParameterByName("u_SkyBoxCcubeTex") = c_cube_map;
					*effect->getParameterByName("u_IntegratedBRDFTex") = integratedBRDFTex;
					mSpheres[i * spheres_column + j]->addToSceneManager();
				}
			}
			mSkyBox = MakeSharedPtr<SceneObjectSkyBox>(0);
			checked_pointer_cast<SceneObjectSkyBox>(mSkyBox)->setCubeMap(sky_box_tex);
			mSkyBox->addToSceneManager();

			mTerrainBlock = MakeSharedPtr<SceneObject>(SceneObject::SOA_Cullable);
			StaticMeshPtr terrain = SimpleMeshFactory::createStaticQuat();
			/*RenderEffectPtr effect = syncLoadRenderEffect("assets/shader/simpleforward.asd");
			terrain->setTechnique(effect, effect->getTechniqueByName("forwardRendering"));*/
			terrain->setMaterial(MakeSharedPtr<RenderMaterial>(float3(1.0f, 1.0f, 1.0f), float3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f));
			mTerrainBlock->setRenderable(terrain);
			mTerrainBlock->setLocalMatrix(MathLib::scaling(float3(50, 50, 50)) * MathLib::translation(float3(1, 1, 1)) * MathLib::to_matrix(MathLib::rotation_axis(float3(1, 0, 0), PI / 4)));
			//mTerrainBlock->addToSceneManager();
			this->LookAt(float3(0.0f, 0.0f, -10.0f), float3(0.0f, 0.0f, 0.0f));
			this->Proj(0.05f, 500);
			mCameraController.attachCamera(this->getActiveCamera());
			mCameraController.setScalers(0.003f, 0.003f);

			InputEngine& inputEngine = SingletonManager::getInputFactoryInstance().getInputEngineInstance();
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
		Air::SceneObjectPtr mTerrainBlock;
		SceneObjectPtr mSkyBox;
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