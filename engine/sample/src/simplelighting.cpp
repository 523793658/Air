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

	float4 diff_parametes[] =
	{
		float4(0.0147f,  0.0332f,  0.064f,   1),
		float4(0.0183f,  0.0657f,  0.0248f,  1),
		float4(0.0712f,  0.0263f,  0.03f,    1),
		float4(0.314f,   0.259f,   0.156f,   1),
		float4(0.226f,   0.233f,   0.0489f,  1),
		float4(0.0864f,  0.0597f,  0.0302f,  1),
		float4(0.31f,    0.248f,   0.151f,   1),
		float4(0.0335f,  0.028f,   0.00244f, 1),
		float4(0.0633f,  0.0603f,  0.0372f,  1),
		float4(0.00289f, 0.00263f, 0.00227f, 1),
		float4(0.000547f,0.00133f, 0.0013f,  1),
		float4(0.0192f,  0.0156f,  0.0104f,  1),
		float4(0.00272f, 0.0022f,  0.00169f, 1),
		float4(0.0173f,  0.0142f,  0.00972f, 1),
		float4(0.00793f, 0.0134f,  0.0198f,  1),
		float4(0.0425f,  0.0698f,  0.095f,   1),
		float4(0.0382f,  0.0272f,  0.0119f,  1),
		float4(0.0173f,  0.0221f,  0.0302f,  1),
		float4(0.1f,     0.0613f,  0.0223f,  1),
		float4(0.0633f,  0.0452f,  0.0226f,  1),
		float4(0.0119f,  0.025f,   0.00997f, 1),
		float4(0.0837f,  0.0128f,  0.00775f, 1),
		float4(0.0432f,  0.0167f,  0.00699f, 1),
		float4(0.00817f, 0.0063f,  0.00474f, 1),
		float4(0.0299f,  0.0273f,  0.0196f,  1),
		float4(0.0916f,  0.027f,   0.00942f, 1),
		float4(0.0749f,  0.0414f,  0.027f,   1),
		float4(0.012f,   0.0143f,  0.0267f,  1),
		float4(0.256f,   0.0341f,  0.0102f,  1),
		float4(0.299f,   0.249f,   0.15f,    1),
		float4(0.314f,   0.183f,   0.0874f,  1),
		float4(0.25f,    0.148f,   0.088f,   1),
		float4(0.0122f,  0.0058f,  0.00354f, 1),
		float4(0.0232f,  0.0216f,  0.0349f,  1),
		float4(0.0474f,  0.0375f,  0.0302f,  1),
		float4(0.0731f,  0.0894f,  0.0271f,  1),
		float4(0.247f,   0.0676f,  0.0414f,  1),
		float4(0.296f,   0.182f,   0.12f,    1),
		float4(0.191f,   0.0204f,  0.00426f, 1),
		float4(0.0515f,  0.0327f,  0.0141f,  1),
		float4(0.164f,   0.0796f,  0.0205f,  1),
		float4(0.102f,   0.0887f,  0.0573f,  1),
		float4(0.00727f, 0.0219f,  0.0132f,  1),
		float4(0.00479f, 0.0318f,  0.0267f,  1),
		float4(0.016f,   0.0701f,  0.0538f,  1),
		float4(0.0307f,  0.0267f,  0.0186f,  1),
		float4(0.0591f,  0.0204f,  0.0062f,  1),
		float4(0.152f,   0.023f,   0.00514f, 1),
		float4(0.191f,   0.0302f,  0.0187f,  1),
		float4(0.0152f,  0.00973f, 0.0177f,  1),
		float4(0.069f,   0.0323f,  0.00638f, 1),
		float4(0.0695f,  0.0628f,  0.0446f,  1),
		float4(0.102f,   0.036f,   0.00995f, 1),
		float4(0.252f,   0.186f,   0.106f,   1),
		float4(0.0301f,  0.0257f,  0.0173f,  1),
		float4(0.236f,   0.204f,   0.127f,   1),
		float4(0.325f,   0.0469f,  0.00486f, 1),
		float4(0.096f,   0.0534f,  0.0184f,  1),
		float4(0.41f,    0.124f,   0.00683f, 1),
		float4(0.00198f, 0.0022f,  0.00203f, 1),
		float4(0.418f,   0.0415f,  0.00847f, 1),
		float4(0.181f,   0.129f,   0.0776f,  1),
		float4(0.29f,    0.161f,   0.0139f,  1),
		float4(0.189f,   0.146f,   0.0861f,  1),
		float4(0.288f,   0.18f,    0.0597f,  1),
		float4(0.146f,   0.0968f,  0.0559f,  1),
		float4(0.201f,   0.109f,   0.0599f,  1),
		float4(0.388f,   0.0835f,  0.043f,   1),
		float4(0.267f,   0.236f,   0.161f,   1),
		float4(0.0555f,  0.0578f,  0.0432f,  1),
		float4(0.0194f,  0.0152f,  0.0105f,  1),
		float4(0.0876f,  0.0322f,  0.0165f,  1),
		float4(0.00498f, 0.00255f, 0.00151f, 1),
		float4(0.289f,   0.22f,    0.13f,    1),
		float4(0.0275f,  0.00723f, 0.00123f, 1),
		float4(0.273f,   0.0276f,  0.0186f,  1),
		float4(0.0316f,  0.0308f,  0.0238f,  1),
		float4(0.302f,   0.0316f,  0.00636f, 1),
		float4(0.132f,   0.0182f,  0.00668f, 1),
		float4(0.00568f, 0.00249f, 0.00118f, 1),
		float4(0.167f,   0.0245f,  0.00789f, 1),
		float4(0.276f,   0.0456f,  0.0109f,  1),
		float4(0.242f,   0.0316f,  0.00946f, 1),
		float4(0.161f,   0.0841f,  0.0537f,  1),
		float4(0.0146f,  0.011f,   0.00606f, 1),
		float4(0.021f,   0.0162f,  0.0106f,  1),
		float4(0.0303f,  0.0187f,  0.0122f,  1),
		float4(0.0156f,  0.0162f,  0.0112f,  1),
		float4(0.345f,   0.291f,   0.196f,   1),
		float4(0.303f,   0.261f,   0.178f,   1),
		float4(0.026f,   0.0172f,  0.00442f, 1),
		float4(0.0708f,  0.0167f,  0.013f,   1),
		float4(0.245f,   0.053f,   0.0749f,  1),
		float4(0.00321f, 0.00218f, 0.00141f, 1),
		float4(0.284f,   0.196f,   0.075f,   1),
		float4(0.317f,   0.234f,   0.107f,   1),
		float4(0.312f,   0.265f,   0.178f,   1),
		float4(0.307f,   0.118f,   0.0101f,  1),
		float4(0.293f,   0.104f,   0.0162f,  1),
		float4(0.253f,   0.187f,   0.0263f,  1)
	};

	float4 spec_parameters[] =
	{
		float4(0.0016f,		0.00115f,	0.000709f,	1),
		float4(0.00161f,	0.00121f,	0.000781f,	1),
		float4(0.000956f,	0.000608f,	0.000389f,	1),
		float4(0.00103f,	0.000739f,	0.000481f,	1),
		float4(0.00135f,	0.000975f,	0.000641f,	1),
		float4(0.015f,		0.00818f,	0.00381f,	1),
		float4(0.00206f,	0.00133f,	0.00171f,	1),
		float4(0.0243f,		0.0187f,	0.00893f,	1),
		float4(0.00499f,	0.00443f,	0.00246f,	1),
		float4(0.00253f,	0.00166f,	0.000999f,	1),
		float4(0.00211f,	0.00148f,	0.000854f,	1),
		float4(0.00692f,	0.00559f,	0.00365f,	1),
		float4(0.00684f,	0.00508f,	0.00374f,	1),
		float4(0.0014f,		0.00114f,	0.000769f,	1),
		float4(0.000914f,	0.000609f,	0.000417f,	1),
		float4(0.00533f,	0.00471f,	0.00333f,	1),
		float4(0.0367f,		0.015f,		0.00537f,	1),
		float4(0.00854f,	0.0115f,	0.0152f,	1),
		float4(0.0396f,		0.019f,		0.00728f,	1),
		float4(0.0459f,		0.0342f,	0.0193f,	1),
		float4(0.00201f,	0.00428f,	0.00175f,	1),
		float4(0.0096f,		0.00153f,	0.000535f,	1),
		float4(0.0092f,		0.00786f,	0.00539f,	1),
		float4(0.0213f,		0.0151f,	0.00766f,	1),
		float4(0.0235f,		0.0169f,	0.00922f,	1),
		float4(0.00946f,	0.00753f,	0.00509f,	1),
		float4(0.0756f,		0.0437f,	0.0202f,	1),
		float4(0.0074f,		0.00513f,	0.00364f,	1),
		float4(0.00473f,	0.00271f,	0.00194f,	1),
		float4(0.00755f,	0.00654f,	0.00461f,	1),
		float4(0.00149f,	0.000803f,	0.000127f,	1),
		float4(0.000481f,	0.000186f,	0.000052f,	1),
		float4(0.000109f,	0.0000582f, 0.0000441f, 1),
		float4(0.000272f,	0.000254f,	0.000286f,	1),
		float4(0.00177f,	0.00223f,	0.00172f,	1),
		float4(0.000423f,	0.000389f,	0.000147f,	1),
		float4(0.0218f,		0.00913f,	0.00523f,	1),
		float4(0.0118f,		0.00811f,	0.00272f,	1),
		float4(0.00634f,	0.00133f,	0.00056f,	1),
		float4(0.0107f,		0.00876f,	0.00573f,	1),
		float4(0.0392f,		0.0265f,	0.0107f,	1),
		float4(0.00699f,	0.00566f,	0.0036f,	1),
		float4(0.00247f,	0.00158f,	0.00107f,	1),
		float4(0.0126f,		0.019f,		0.0138f,	1),
		float4(0.000998f,	0.00068f,	0.000413f,	1),
		float4(0.0269f,		0.0234f,	0.0163f,	1),
		float4(0.0102f,		0.00878f,	0.00617f,	1),
		float4(0.00197f,	0.00116f,	0.000782f,	1),
		float4(0.00289f,	0.00171f,	0.00117f,	1),
		float4(0.0216f,		0.0177f,	0.0291f,	1),
		float4(0.0738f,		0.0434f,	0.0104f,	1),
		float4(0.0742f,		0.0615f,	0.0412f,	1),
		float4(0.0117f,		0.00997f,	0.00676f,	1),
		float4(0.00732f,	0.00617f,	0.00381f,	1),
		float4(0.0665f,		0.0486f,	0.0285f,	1),
		float4(0.00572f,	0.00479f,	0.0031f,	1),
		float4(0.00125f,	0.00069f,	0.000432f,	1),
		float4(0.0057f,		0.00486f,	0.00313f,	1),
		float4(0.00928f,	0.00433f,	0.00317f,	1),
		float4(0.0127f,		0.0104f,	0.00679f,	1),
		float4(0.00735f,	0.00462f,	0.00362f,	1),
		float4(0.0443f,		0.0389f,	0.0276f,	1),
		float4(0.014f,		0.0115f,	0.00616f,	1),
		float4(0.0485f,		0.0346f,	0.0161f,	1),
		float4(0.00477f,	0.00387f,	0.00252f,	1),
		float4(0.00986f,	0.00855f,	0.00584f,	1),
		float4(0.00494f,	0.00423f,	0.00294f,	1),
		float4(0.00137f,	0.00204f,	0.00146f,	1),
		float4(0.00362f,	0.00425f,	0.00259f,	1),
		float4(0.00934f,	0.00771f,	0.00512f,	1),
		float4(0.000311f,	0.000266f,	0.000179f,	1),
		float4(0,			0,			0,			1),
		float4(0.00536f,	0.00443f,	0.00304f,	1),
		float4(0.00497f,	0.00421f,	0.0021f,	1),
		float4(0.0131f,		0.0111f,	0.00757f,	1),
		float4(0.00872f,	0.0069f,	0.00475f,	1),
		float4(0.00814f,	0.00619f,	0.00395f,	1),
		float4(0.00093f,	0.000782f,	0.000514f,	1),
		float4(0.00817f,	0.00571f,	0.00394f,	1),
		float4(0.0117f,		0.0101f,	0.00699f,	1),
		float4(0.00239f,	0.00185f,	0.00117f,	1),
		float4(0.00801f,	0.00662f,	0.00463f,	1),
		float4(0.00237f,	0.00156f,	0.000996f,	1),
		float4(0.00241f,	0.00181f,	0.0011f,	1),
		float4(0.00394f,	0.00299f,	0.00319f,	1),
		float4(0.011f,		0.00946f,	0.00645f,	1),
		float4(0.0232f,		0.0161f,	0.00974f,	1),
		float4(0.0189f,		0.0103f,	0.00561f,	1),
		float4(0,			0,			0,			1),
		float4(0.01f,		0.009f,		0.00649f,	1),
		float4(0.0268f,		0.0157f,	0.012f,		1),
		float4(0.00109f,	0.000602f,	0.000442f,	1),
		float4(0.00649f,	0.00455f,	0.00342f,	1),
		float4(0.0117f,		0.0102f,	0.00705f,	1),
		float4(0.001f,		0.000612f,	0.000465f,	1),
		float4(0.00716f,	0.00615f,	0.00379f,	1),
		float4(0.0068f,		0.00514f,	0.00358f,	1),
		float4(0.000493f,	0.000338f,	0.000288f,	1),
		float4(0.00424f,	0.00333f,	0.00211f,	1),
		float4(0.00376f,	0.00389f,	0.00213f,	1)
	};

	float glossiness_parametes[] =
	{
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		0.945208645f,
		0.458015101f,
		0.910586764f,
		0.583458654f,
		1,
		0.418942787f,
		1,
		1,
		0.943868871f,
		0.48518681f,
		1,
		1,
		0.489177717f,
		1,
		1,
		0.459261041f,
		1,
		0.382160827f,
		0.391669218f,
		0.490921191f,
		0.49850679f,
		0.562529458f,
		0.490521275f,
		0.525606924f,
		0.332456007f,
		0.610056718f,
		0.257730557f,
		0.284649209f,
		0.358103987f,
		0.541032539f,
		0.400125682f,
		0.77095137f,
		1,
		0.474609615f,
		1,
		1,
		0.493160556f,
		1,
		1,
		0.407419801f,
		0.414529103f,
		0.479139899f,
		0.502892822f,
		0.490387649f,
		0.77095137f,
		0.596014835f,
		1,
		1,
		0.353610396f,
		0.695722625f,
		0.380012827f,
		0.409101295f,
		0.244086726f,
		0.368601082f,
		0.930769633f,
		0.495355626f,
		0.828703016f,
		0.388366101f,
		0.346997071f,
		0.612307841f,
		0.508142297f,
		0.041234838f,
		0.581122219f,
		0.404559422f,
		0.541876471f,
		0.596014835f,
		0.65685837f,
		1,
		0.472901056f,
		0.514346194f,
		1,
		0.409932584f,
		1,
		0.94454078f,
		1,
		0.90351341f,
		1,
		1,
		0.001104253f,
		0.459966777f,
		1,
		1,
		0.419956278f,
		0.631496413f,
		1,
		0.487817693f,
		0.689453539f,
		1,
		0.791362491f,
		0.423187627f
	};


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
			ambient_light->setColor(float3(1.0f, 1.0f, 1.0f) * 0.01f);
			ambient_light->setSkyLightTexC(c_cube_map);
			ambient_light->setSkyLightTexY(y_cube_map);
			ambientLightObject->setCustomData(ambient_light);
			ambientLightObject->addToSceneManager();


			DirectLightSourcePtr direction_light = MakeSharedPtr<DirectLightSource>();
			direction_light->setColor(float3(1, 1, 1) * 1.6f);
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
					mSpheres[i * spheres_column + j]->setLocalMatrix(MathLib::translation(1.1f*(-static_cast<float>(spheres_column / 2) + j + 0.5f),
						0.0f,
						1.1f * (-static_cast<float>(spheres_row / 2) + i + 0.5f)));
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
			RenderEffectPtr effect = syncLoadRenderEffect("assets/shader/simpleforward.asd");
			//terrain->setTechnique(effect, effect->getTechniqueByName("forwardRendering"));
			*effect->getParameterByName("u_SkyBoxTex") = y_cube_map;
			*effect->getParameterByName("u_SkyBoxCcubeTex") = c_cube_map;
			*effect->getParameterByName("u_IntegratedBRDFTex") = integratedBRDFTex;
			terrain->bindEffect(effect);
			terrain->setMaterial(MakeSharedPtr<RenderMaterial>(float3(1.0f, 1.0f, 1.0f), float3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f));
			mTerrainBlock->setRenderable(terrain);
			mTerrainBlock->setLocalMatrix(MathLib::scaling(float3(50, 50, 50)) * MathLib::to_matrix(MathLib::rotation_axis(float3(1, 0, 0), -PI / 2))* MathLib::translation(float3(0, -1, 0)));
			mTerrainBlock->addToSceneManager();


			this->LookAt(float3(0.0f, 0.0f, -10.0f), float3(0.0f, 0.0f, 0.0f));
			this->Proj(0.05f, 200);
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