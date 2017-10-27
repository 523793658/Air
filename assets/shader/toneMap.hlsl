#ifndef __ToneMap_Include__
#define __ToneMap_Include__

void toneMapVS(float4 pos : POSITION,
	float2 texcoord0 : TEXCOORD0,
	out float2 vTexcoord0 : TEXCOORD0,
	out float4 oPos : SV_Position)
{
	oPos = pos;
	vTexcoord0 = texcoord0;
}

float4 toneMapPS(float2 texcoord0 : TEXCOORD0) : SV_Target
{
	return u_SceneTex.Sample(s_SceneTexSampler, texcoord0);
}





#endif