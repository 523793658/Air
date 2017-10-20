#ifndef __SkyBox_Include__
#define __SkyBox_Include__
#include "util.hlsl"

void skyBoxVS(float4 pos : POSITION,
	out float3 texcoord0 : TEXCOORD0,
	out float4 oPos : SV_Position)
{
	oPos = pos;
	texcoord0 = mul(pos, u_InvMVP).xyz;
}

float4 skyBoxPS(float3 texcoord0 : TEXCOORD0) : SV_Target
{
	if (u_SkyBoxCompressed)
	{
		return decode_hdr_yc(u_SkyBoxTex.Sample(s_SkyboxSampler, texcoord0).r,
			u_SkyBoxCcubeTex.Sample(s_SkyboxSampler, texcoord0));
	}
	else
	{
		return u_SkyBoxCcubeTex.Sample(s_SkyboxSampler, texcoord0);
	}
}


#endif