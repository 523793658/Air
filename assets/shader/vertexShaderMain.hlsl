#ifndef _VertexShaderMain_H_
#define _VertexShaderMain_H_

#ifdef POST_PROCESS

void postProcessVertexShader(float4 pos : POSITION,
	float2 texcoord0 : TEXCOORD0,
	out float2 vTexcoord0 : TEXCOORD0,
	out float4 oPos : SV_Position)
{
	oPos = pos;
	vTexcoord0 = texcoord0;
}
#endif


#ifdef STANDARDSURFACE


struct VS_INPUT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};
struct VS_OUTPUT
{
	float4 oPos : SV_Position;
	float2 vTexcoord : TEXCOORD0;
	float3 vNormal : NORMAL0;
	float4 vWorldPos : TEXCOORD1;
	float4 vProjPos : TEXCOORD2;
};



VS_OUTPUT defaultModelVertexShader(VS_INPUT input)
{
	VS_OUTPUT output;
	output.oPos = mul(input.pos, mvp);
	output.vNormal = mul(input.normal, (float3x3)worldMatrix);
	output.vTexcoord = input.tex;
	output.vWorldPos = mul(input.pos, worldMatrix);
	output.vProjPos = output.oPos;
	return output;
}
#endif








#endif