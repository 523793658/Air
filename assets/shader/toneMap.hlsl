#ifndef __ToneMap_Include__
#define __ToneMap_Include__

#include "vertexShaderMain.hlsl"
static const float3 BLUE_SHIFT = float3(0.4f, 0.4f, 0.7f);
float EyeAdaption(float lum)
{
	return lerp(0.2f, lum, 0.5f);
}
static const float A = 2.51f;
static const float B = 0.03f;
static const float C = 2.43f;
static const float D = 0.59f;
static const float E = 0.14f;
float3 ACESFilm(float3 x)
{
	return (x * (A * x + B)) / (x * (C * x + D) + E);
}

float3 toneMap(float3 color, float3 blur, float adapted_lum)
{
	color += blur * u_Params.x;
	if (u_BlueShift)
	{
		float lum = dot(color, RGB_TO_LUM);
		color = lerp(lum * BLUE_SHIFT, color, saturate(16.0f * lum));
	}
	float adapted_lum_dest = 2 / (max(0.1f, 1 + 10 * EyeAdaption(adapted_lum)));
	//return ACESFilm(adapted_lum_dest * u_Params.y * color);
	return color;
}

void tonemappingVS(float4 pos : POSITION,
	float2 texcoord : TEXCOORD0,
	out float3 v_Texcoord : TEXCOORD0, 
	out float4 oPos : SV_Position)
{
	oPos = pos;
	v_Texcoord.xy = texcoord;
	float lum = u_LuminanceTex.SampleLevel(s_PointSampler, 0.5f, 0).r;
	v_Texcoord.z = max(0.001f, lum);
}

float4 toneMapPS(float3 v_Texcoord : TEXCOORD0) : SV_Target
{
	float3 color = toneMap(u_SceneTex.SampleLevel(s_LinearSampler, v_Texcoord.xy, 0).rgb, 0.0f, v_Texcoord.z);
	return float4(color, 1.0f);
}





#endif