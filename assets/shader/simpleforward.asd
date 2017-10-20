<?xml version='1.0'?>

<effect>
	<include name="./assets/shader/lighting.asd"/>


    <cbuffer name="cb_RenderEnvironment" type="CBT_Global">
        <parameter name="u_DirLightDir" type="float3"/>
        <parameter name="u_DirLightColor" type="float3"/>
        <parameter type="float3" name="u_CameraDir"/>
    </cbuffer>

    <parameter type="float4x4" name="mvp"/>
	<parameter type="float4x4" name="worldMatrix"/>
    <parameter type="float4" name="u_BaseColorRoughness"/>
    <parameter type="float4" name="u_SpecularColorMetallic"/>
	<shader>
		<![CDATA[

    
struct FGBufferData
{
    float3 worldNormal;
    float3 diffuseColor;
    float3 specularColor;
    float3 baseColor;
    float metallic;
    float specular;
    float roughness;
    float4 customData;
 
    uint shadingModelID;
};

#include "common.hlsl"
#include "shadingModels.hlsl"
		
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

struct PS_OUTPUT
{
	float4 color : SV_Target;
};
		
VS_OUTPUT simplestVS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.oPos = mul(input.pos, mvp);
	output.vNormal = mul(input.normal, (float3x3)worldMatrix);
	return output;
}

struct FLightData
{
    float3 direction;
};


float3 areaLightSpecular(inout float3 lobeRoughness, inout float3 toLight, inout float3 L, float3 V, float3 N)
{
    float3 lobeEnergy = 1;
    float3 m = lobeRoughness * lobeRoughness;
    float3 R = reflect(-V, N);
    float InvDistToLight = rsqrt(dot(toLight, toLight));
    float a = square(lobeRoughness[1]);
    R = lerp(N, R, (1 - a) * (sqrt(1 - a) + a));
    R = normalize(R);
    
    L = normalize(toLight);
    return lobeEnergy;
}		
PS_OUTPUT simplestPS(VS_OUTPUT input)
{
	PS_OUTPUT output;
    
    float3 localSpecular = u_SpecularColorMetallic.rgb;
    float3 localBaseColor = u_BaseColorRoughness.rgb;
    
    FGBufferData GBuffer;
    GBuffer.roughness = u_BaseColorRoughness.w;
    GBuffer.metallic = u_SpecularColorMetallic.w;
    GBuffer.baseColor = localBaseColor;
    GBuffer.specularColor = lerp(0.08 * localSpecular.xxx, localSpecular, GBuffer.metallic.xxx);
    GBuffer.diffuseColor = localBaseColor - localBaseColor * GBuffer.metallic;
    GBuffer.customData = 1.0;
    float3 V = -u_CameraDir;
    float3 N = normalize(input.vNormal);
    float3 L = u_DirLightDir;
    float3 toLight = L;
    
    float NoL = saturate(dot(N, L));
    
    
    float3 lobeRoughness = float3(GBuffer.customData.y, GBuffer.roughness, 1);
    float3 lobeEnergy = areaLightSpecular(lobeRoughness, toLight, L, V, N); 
    float3 color = surfaceShading(GBuffer, lobeRoughness, lobeEnergy, L, V, N, uint2(0, 0));
    float3 CommonMultiplier = u_DirLightColor * NoL;
    output.color = float4(color * CommonMultiplier, 1.0);
    //output.color.rgb = color;
	return output;
}
		
		
		]]>
	</shader>
	<technique name="simplest">
		<pass name="p0">
			<state name = "vertex_shader" value="simplestVS()"/>
			<state name = "pixel_shader" value="simplestPS()"/>
			<state name = "cull_mode" value="back" />
		</pass>
	</technique>
</effect>