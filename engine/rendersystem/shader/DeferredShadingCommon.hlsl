#ifndef _DefeeredShadingCommon_HLSL_
#define _DefeeredShadingCommon_HLSL_

struct FGBufferData
{
	float3 WorldNormal;
	float3 DiffuseColor;
	float3 SpecularColor;
	float3 BaseColor;
	float Metallic;
	float Specular;
	float4 CustomData;
	float IndirectIrradiance;
	float4 PrecomputedShadowFactors;
	float Roughness;
	float GBufferAO;
	uint ShadingModelID;
	float CustomDepth;
	uint CustomStencil;
	float Depth;
	float4 Velocity;
	float3 StoredBaseColor;
	float StoredSpecular;
};



#endif