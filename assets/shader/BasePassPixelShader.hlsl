#include "common.hlsl"
#include "../materials/simplestMaterial.hlsl"
#include "BasePassCommon.hlsl"
#include "LocalVertexFactory.hlsl"
#include "LightAccumulator.hlsl"
#include "DeferredShadingCommon.hlsl"


void GetPrecomputedIndirectLightingAndSkyLight(
	FMaterialPixelParameters MaterialParameters,
	FVertexFactoryInterpolantsVSToPS Interpolants,
	FBasePassInterpolantsVSToPS BasePassInterpolants,
	float3 DiffuseDir,
	out float3 OutDiffuseLighting,
	out float3 OutSubsurfaceLighting,
	out float OutIndirectIrradiance)
{
	OutIndirectIrradiance = 0;
	OutDiffuseLighting = 0;
	OutSubsurfaceLighting = 0;
	float2 SkyOcclusionUV = 0;
}


void FPixelShaderInOut_MainPS(
	FVertexFactoryInterpolantsVSToPS Interpolants, 
	FBasePassInterpolantsVSToPS BasePassInterpolants,
	in FPixelShaderIn In,
	inout FPixelShaderOut Out)
{
	FMaterialPixelParameters MaterialParameters = GetMaterialPixelParameters(Interpolants, In.vPosition);
	FPixelMaterialInputs PixelMaterialInputs;

	float4 ScreenPosition = vPositionToResolveScreenPosition(In.vPosition);
	float3 TranslateWorldPosition = vPositionToResolvedTranslatedWorld(In.vPosition);
	CalcMaterialParametersEx(MaterialParameters, PixelMaterialInputs, In.vPosition, ScreenPosition, In.bIsFrontFace, TranslateWorldPosition, TranslateWorldPosition);


	half3 BaseColor = GetMaterialBaseColor(PixelMaterialInputs);
	half Metallic = GetMaterialMetallic(PixelMaterialInputs);
	half Specular = GetMaterialSpecular(PixelMaterialInputs);
	float MaterialAO = GetMaterialAmbientOcclusion(PixelMaterialInputs);
	float Roughness = GetMaterialRoughness(PixelMaterialInputs);

	float3 LocalBaseColor = BaseColor;
	float LocalSpecular = Specular;


	FGBufferData GBuffer = (FGBufferData)0;
	GBuffer.WorldNormal = MaterialParameters.WorldNormal;
	GBuffer.BaseColor = BaseColor;
	GBuffer.Metallic = Metallic;
	GBuffer.Specular = Specular;
	GBuffer.Roughness = Roughness;
	GBuffer.GBufferAO = MaterialAO;
	GBuffer.Depth = MaterialParameters.ScreenPosition.w;
#if MATERIAL_SHADINGMODEL_DEFAULT_LIT
	GBuffer.ShadingModelID = SHADINGMODELID_DEFAULT_LIT;
#else

#endif

	GBuffer.SpecularColor = lerp(0.08 * LocalSpecular.xxx, LocalBaseColor, Metallic.xxx);

	
	
	GBuffer.DiffuseColor = LocalBaseColor - LocalBaseColor * Metallic;

	half3 DiffuseColor = 0;
	half3 Color = 0;
	float indirectIrradiance = 0;
#if !MATERIAL_SHADINGMODEL_UNLIT
	float3 DiffuseDir = MaterialParameters.WorldNormal;
	float3 DiffuseColorForIndirect = GBuffer.DiffuseColor;

	float3 DiffuseIndirectLighting = 0;
	//GetPrecomputedIndirectLightingAndSkyLight(MaterialParameters, Interpolants, BasePassInterpolants, DiffuseDir, DiffuseIndirectLighting, 0, IndirectIrradiance);

	float IndirectOcclusion = 1.0f;
	float2 NearestResolvedDepthScreenUV = 0;

	DiffuseColor += (DiffuseIndirectLighting * DiffuseColorForIndirect) * MaterialAO;

	half3 Emissive = GetMaterialEmissive(PixelMaterialInputs);
#endif

#if NEEDS_BASEPASS_FOGGING
	float4 VertexFog = BasePassInterpolants.VertexFog;
#else
	float4 VertexFog = float4(0, 0, 0, 1);
#endif

	Color += DiffuseColor;
	Color += Emissive;


#if MATERIALBLENDING_ALPHACOMPOSITE
#elif MATERIALBLENDING_TRANSLUCENT
#elif MATERIALBLENDING_ADDITIVE
#elif MATERIALBLENDING_MODULATE
#else
	{
		FLightAccumulator LightAccumulator = (FLightAccumulator)0;
		Color = Color * VertexFog.a + VertexFog.rgb;
		LightAccumulator_Add(LightAccumulator, Color, 0, 1.0f, false);
		Out.MRT[0] = RETURN_COLOR(LightAccumulator_GetResult(LightAccumulator));
	}
#endif

#if USES_GBUFFER
	GBuffer.IndirectIrradiance = IndirectIrradiance;
	float QuantizationBias = PseudoRandom(MaterialParameters.vPosition.xy) - 0.5f;
	EncodeGBuffer(GBuffer, Out.MRT[1], Out.MRT[2], Out.MRT[3], OutGBufferD, OutGBufferE, OutVelocity, QuantizationBias);
#endif

#if USES_GBUFFER
#if GBUFFER_HAS_VELOCITY
	Out.MRT[4] = OutVelocity;
#endif
	Out.MRT[GBUFFER_HAS_VELOCITY ? 5 : 4] = OutGBufferD;
#if GBUFFER_HAS_PRECSHADOWFACTOR
	Out.MRT[GBUFFER_HAS_VELOCITY ? 6 : 5] = OutGBufferE;
#endif
#endif

}




#define PIXELSHADEROUTPUT_BASEPASS 1

#define PIXELSHADEROUTPUT_MRT0 (!USE_GBUFFER || !SELECTIVE_BASEPASS_OUTPUTS || NEEDS_BASEPASS_FOGGING || USES_EMISSIVE_COLOR || ALLOW_STATIC_LIGHTING)


#include "PixelShaderOutputCommon.hlsl"