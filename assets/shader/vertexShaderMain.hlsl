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
	float3 normal : NORMAL;
};
struct VS_OUTPUT
{
	float4 oPos : SV_Position;
	float3 vNormal : NORMAL0;
};



VS_OUTPUT defaultModelVertexShader(VS_INPUT input)
{
	VS_OUTPUT output;
	output.oPos = mul(input.pos, mvp);
	output.vNormal = mul(input.normal, (float3x3)worldMatrix);
	return output;
}
#endif








#endif