#ifndef __Common_Include__
#define __Common_Include__

const static float PI = 3.1415926535897932f;

#if (!(COMPILER_GLSL_ES2 || COMPILER_GLSL_ES3_1) && !METAL_PROFILE) || FORCE_FLOATS
// Always use floats when not using the ES2 compiler, because low precision modifiers are currently only tweaked for ES2, 
// And we don't want potential side effects on other platforms
#define half float
#define half1 float1
#define half2 float2
#define half3 float3
#define half4 float4
#define half3x3 float3x3
#define half4x4 float4x4 
#define half4x3 float4x3 
#define fixed float
#define fixed1 float1
#define fixed2 float2
#define fixed3 float3
#define fixed4 float4
#define fixed3x3 float3x3
#define fixed4x4 float4x4
#define fixed4x3 float4x3
#endif

#include "InstancedStereo.hlsl"
#include "Definitions.hlsl"

#define FEATURE_LEVEL_ES2	1
#define FEATURE_LEVEL_ES3_1	2
#define FEATURE_LEVEL_SM3	3
#define FEATURE_LEVEL_SM4	4
#define FEATURE_LEVEL_SM5	5
#define FEATURE_LEVEL_MAX	6

float square(float x)
{
	return x*x;
}

float2 square(float2 x)
{
	return x* x;
}

float3 square(float3 x)
{
	return x * x;
}
float4 square(float4 x)
{
	return x * x;
}

float pow4(float x)
{
	float xx = x * x;
	return xx * xx;
}

float pow5(float x)
{
	float xx = x * x;
	return xx * xx * x;
}

float2 pow5(float2 x)
{
	float2 xx = x * x;
	return xx * xx * x;
}

float3 pow5(float3 x)
{
	float3 xx = x * x;
	return xx * xx * x;
}

float4 pow6(float4 x)
{
	float4 xx = x * x;
	return xx * xx * x;
}

struct FPixelShaderIn
{
	float4 vPosition;
	uint Coverage;
	bool bIsFrontFace;
};

struct FPixelShaderOut
{
	float4 MRT[8];
	uint Coverage;
	float Depth;
};



#define RETURN_COLOR(Color) (Color)

float4 vPositionToResolveScreenPosition(float4 vPosition)
{
	float2 PixelPos = vPosition.xy - ResolvedView.ViewRectMin.xy;
	float3 NDCPos = float3((PixelPos * ResolvedView.ViewSizeAndInvSize.zw - 0.5f) * float2(2, -2), vPosition.z);
	return float4(NDCPos.xyz, 1) * vPosition.w;
}

float3 vPositionToResolvedTranslatedWorld(float4 vPosition)
{
	float4 HomWorldPos = mul(float4(vPosition.xyz, 1), ResolvedView.SVPositionToTranslatedWorld);
	return HomWorldPos.xyz / HomWorldPos.w;
}





#endif