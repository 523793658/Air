#ifndef _ShadowPCF_Include_H_
#define _ShadowPCF_Include_H_





void genShadowMapVS(float4 g_Position : POSITION0, out float4 v_Position : SV_Position)
{
	v_Position = mul(g_Position, mvp);
}

float4 genShadowMapPS() : SV_Target
{
	return 1.0;
}






#endif