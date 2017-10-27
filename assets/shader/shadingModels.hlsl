#ifndef __ShadingModels_Include__
#define __ShadingModels_Include__
#include "brdf.hlsl"

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
	return (D * Vis) *F;
}


float3 surfaceShading(FGBufferData GBuffer, float3 lobeRoughness, float3 lobeEnergy, float3 L, float3 V, half3 N, uint2 random)
{
	switch (GBuffer.shadingModelID)
	{
	default:
		return standardShading(GBuffer.diffuseColor, GBuffer.specularColor, lobeRoughness, lobeEnergy, L, V, N, random);
	}
}

#endif