#ifndef __ShadingModels_Include__
#define __ShadingModels_Include__
#include "util.hlsl"
#include "common.hlsl"
#include "brdf.hlsl"
#include "vertexShaderMain.hlsl"
struct FGBufferData
{
	float3 worldNormal;
	float3 diffuseColor;
	float3 specularColor;
	float metallic;
	float specular;
	float roughness;
	float4 customData;

	uint shadingModelID;
};

float3 standardShading(float3 diffuseColor, float3 specularColor, float3 lobeRoughness, float3 lobeEnergy, float3 L, float3 V, half3 N, uint2 random)
{
	float3 H = normalize(V + L);
	float NoL = saturate(dot(N, L));
	float NoV = saturate(abs(dot(N, V)) + 1e-5);
	float NoH = saturate(dot(N, H));
	float VoH = saturate(dot(V, H));
	float D = D_GGX(lobeRoughness[1], NoH) * lobeEnergy[1];
	float Vis = Vis_SmithJointApprox(lobeRoughness[1], NoV, NoL);
	float3 F = F_Schlick(specularColor, VoH);
	float3 diffuse = diffuse_lambert(diffuseColor);
	return diffuse +(D * Vis) *F;
}


float3 surfaceShading(FGBufferData GBuffer, float3 lobeRoughness, float3 lobeEnergy, float3 L, float3 V, half3 N, uint2 random)
{
	switch (GBuffer.shadingModelID)
	{
	default:
		return standardShading(GBuffer.diffuseColor, GBuffer.specularColor, lobeRoughness, lobeEnergy, L, V, N, random);
	}
}


struct SurfaceOutputStandard
{
	float roughness;
	float metallic;
	float3 baseColor;
	float3 localSpecular;
	float4 viewPosition;
	float4 worldPosition;
	float3 normal;				//这里拿到的应该是法线贴图的值。需要进一步计算才能得到最后的法线（目前线这么久认为是法线吧，后面在改；
};

float3 calcEnvDiffuse(float3 prefiltered_env, float3 diffuseColor)
{
	return prefiltered_env * diffuseColor;
}

#define REFLECTION_CAPTURE_ROUGHEST_MIP 3
#define REFLECTION_CAPTURE_ROUGHNESS_MIP_SCALE 1.15

float calcPrefilteredEnvMip(float roughness, int num_mip)
{
	float LevelFrom1x1 = REFLECTION_CAPTURE_ROUGHEST_MIP - REFLECTION_CAPTURE_ROUGHNESS_MIP_SCALE * log2(roughness);
	return num_mip - 1 - LevelFrom1x1;
}


float3 pBPrefilteredSpecularIBL(float3 specularColor, float roughness, float3 normal, float3 view)
{
	float mip = calcPrefilteredEnvMip(roughness, u_DiffuseSpecularMip.y);
	float NoV = saturate(dot(normal, view));
	float3 r = calcPrefilteredEnvVec(normal, view);
	float3 sampleColor = decode_hdr_yc(u_SkyBoxTex.SampleLevel(s_SkyboxSampler,
		r, mip).r, u_SkyBoxCcubeTex.SampleLevel(s_SkyboxSampler, r, mip)).xyz;
	return sampleColor * envBRDF(specularColor, roughness, NoV);
}

float3 prefilteredDiffuseIBL(float3 diffuseColor, float3 normal)
{
	float3 prefiltered_clr = decode_hdr_yc(u_SkyBoxTex.SampleLevel(s_SkyboxSampler,
		normal, u_DiffuseSpecularMip.x).r, u_SkyBoxCcubeTex.SampleLevel(s_SkyboxSampler, normal, u_DiffuseSpecularMip.x)).xyz;
	return calcEnvDiffuse(prefiltered_clr, diffuseColor);
}

float3 areaLightSpecular(inout float3 lobeRoughness, inout float3 toLight, inout float3 L, float3 V, float3 N)
{
	lobeRoughness = max(lobeRoughness, 0.04);

	float3 lobeEnergy = 1;
	float3 m = lobeRoughness * lobeRoughness;
	float3 R = reflect(-V, N);
	float InvDistToLight = rsqrt(dot(toLight, toLight));
	float a = square(lobeRoughness[1]);
	R = lerp(N, R, (1 - a) * (sqrt(1 - a) + a));
	R = normalize(R);

	L = normalize(toLight);
	return lobeEnergy;
}


float4 standardSurfaceShading(SurfaceOutputStandard input)
{
	FGBufferData GBuffer;
	GBuffer.roughness = input.roughness;

	GBuffer.metallic = input.metallic;

	GBuffer.specularColor = lerp(0.08 * input.localSpecular.xxx, input.localSpecular, GBuffer.metallic);

	GBuffer.diffuseColor = input.baseColor - input.baseColor * GBuffer.metallic;

	GBuffer.customData = 1.0;
	float3 V = -u_CameraDir;
	float3 N = normalize(input.normal);
	float3 L = u_DirLightDir;
	float3 toLight = L;

	float NoL = saturate(dot(N, normalize(L)));


	float3 lobeRoughness = float3(GBuffer.customData.y, GBuffer.roughness, 1);
	float3 lobeEnergy = areaLightSpecular(lobeRoughness, toLight, L, V, N);
	float3 surfaceLighting = surfaceShading(GBuffer, lobeRoughness, lobeEnergy, L, V, N, uint2(0, 0));

	FLightAccumulator lightAccumulator = (FLightAccumulator)0;
	float surfaceAttenuation = 1.0;        //阴影等的削减
	int index = step(u_ViewDistanceClip.x, input.viewPosition.z) + step(u_ViewDistanceClip.y, input.viewPosition.z) + step(u_ViewDistanceClip.z, input.viewPosition.z) + step(u_ViewDistanceClip.w, input.viewPosition.z);
	float4 shadowProPos = mul(input.worldPosition, u_ShadowMatrix[index]);
	float2 texcoord0 = shadowProPos.xy / shadowProPos.w;
	texcoord0.y *= -1;
	float3 texCoord = float3(texcoord0 * 0.5 + 0.5, index);


	float shadow = step(shadowProPos.z / shadowProPos.w - 0.001, u_ShadowMap.Sample(s_ShadowSampler, texCoord).r);

	surfaceAttenuation = shadow;
	lightAccumulatorAdd(lightAccumulator, surfaceLighting, (1.0 / PI), u_DirLightColor * (NoL * surfaceAttenuation), false);

	float3 nonSpecularContribution = prefilteredDiffuseIBL(GBuffer.diffuseColor, N);
	float3 specularContribution = pBPrefilteredSpecularIBL(GBuffer.specularColor, GBuffer.roughness, N, V);

	lightAccumulatorAdd(lightAccumulator, nonSpecularContribution + specularContribution, nonSpecularContribution, u_AmbientCubemapColor, false);
	
	return lightAccumulatorGetResult(lightAccumulator);
}



#endif