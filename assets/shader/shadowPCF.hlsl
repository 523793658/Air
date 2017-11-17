#ifndef _ShadowPCF_Include_H_
#define _ShadowPCF_Include_H_





void genShadowMapVS(float4 g_Position : POSITION0, out float4 v_Position : SV_Position, out float zfar : TEXCOORD0)
{
	v_Position = mul(g_Position, mvp);
	zfar = v_Position.z / v_Position.w;
}

float4 genShadowMapPS(float4 v_Position : SV_Position, float zfar : TEXCOORD0) : SV_Target
{
	float3 color = zfar;
	return float4(color, 1.0);
}






#endif