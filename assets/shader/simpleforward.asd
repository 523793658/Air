<?xml version='1.0'?>

<effect>
	<include name="./assets/shader/lighting.asd"/>


    <cbuffer name="cb_RenderEnvironment" type="CBT_Global">
        <parameter name="u_DirLightDir" type="float3"/>
        <parameter name="u_DirLightColor" type="float3"/>
        <parameter type="float3" name="u_CameraDir"/>
        <parameter type="float3" name="u_AmbientCubemapColor"/>
        <parameter type="int2" name="u_DiffuseSpecularMip"/>
    </cbuffer>



    <parameter type="textureCUBE" name="u_SkyBoxTex"/>
    <parameter type="textureCUBE" name="u_SkyBoxCcubeTex"/>
    <parameter type="sampler" name="s_SkyboxSampler">
        <state name="filtering" value="min_mag_mip_linear"/>
        <state name="address_u" value="clamp"/>
        <state name="address_v" value="clamp"/>
        <state name="address_w" value="clamp"/>
    </parameter>


    <parameter type="texture2D" name="u_IntegratedBRDFTex"/>
    <parameter type="sampler" name="s_BilinearSampler">
        <state name="filtering" value="min_mag_linear_mip_point"/>
        <state name="address_u" value="clamp"/>
        <state name="address_v" value="clamp"/>
        <state name="address_w" value="clamp"/>
    </parameter>

    <parameter type="float4x4" name="mvp"/>
	<parameter type="float4x4" name="worldMatrix"/>
    <parameter type="float4" name="u_BaseColorRoughness"/>
    <parameter type="float4" name="u_SpecularColorMetallic"/>
	<shader>
		<![CDATA[

#include "util.hlsl"
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

float3 calcEnvDiffuse(float3 prefiltered_env, float3 diffuseColor)
{
    return prefiltered_env * diffuseColor;
}
		
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
    lobeRoughness = max(lobeRoughness, 0.04);

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

float3 prefilteredDiffuseIBL(float3 diffuseColor, float3 normal)
{
    float3 prefiltered_clr = decode_hdr_yc(u_SkyBoxTex.SampleLevel(s_SkyboxSampler,
    normal, u_DiffuseSpecularMip.x).r, u_SkyBoxCcubeTex.SampleLevel(s_SkyboxSampler, normal, u_DiffuseSpecularMip.x)).xyz;
    return calcEnvDiffuse(prefiltered_clr, diffuseColor);
}

#define REFLECTION_CAPTURE_ROUGHEST_MIP 1
#define REFLECTION_CAPTURE_ROUGHNESS_MIP_SCALE 1.2

float calcPrefilteredEnvMip(float roughness, int num_mip)
{
    float LevelFrom1x1 = REFLECTION_CAPTURE_ROUGHEST_MIP - REFLECTION_CAPTURE_ROUGHNESS_MIP_SCALE * log2(roughness);
	return num_mip - 1 - LevelFrom1x1;
}

float3 pBPrefilteredSpecularIBL(float3 specularColor, float roughness, float3 normal, float3 view)
{
    float mip = calcPrefilteredEnvMip(roughness, u_DiffuseSpecularMip.y);
    float NoV = saturate(dot(normal, view));
    float3 r = calcPrefilteredEnvVec(normal, view);
    float3 sampleColor = decode_hdr_yc(u_SkyBoxTex.SampleLevel(s_SkyboxSampler,
    r, mip).r, u_SkyBoxCcubeTex.SampleLevel(s_SkyboxSampler, r, mip)).xyz;
    return sampleColor * envBRDF(specularColor, roughness, NoV);
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
    float3 surfaceLighting = surfaceShading(GBuffer, lobeRoughness, lobeEnergy, L, V, N, uint2(0, 0));
      
    FLightAccumulator lightAccumulator = (FLightAccumulator)0;
    float surfaceAttenuation = 1.0;        //阴影等的削减
    lightAccumulatorAdd(lightAccumulator, surfaceLighting, (1.0 / PI), u_DirLightColor * (NoL * surfaceAttenuation), false);
    
    float3 nonSpecularContribution = prefilteredDiffuseIBL(GBuffer.diffuseColor, N);
    float3 specularContribution = pBPrefilteredSpecularIBL(GBuffer.specularColor, GBuffer.roughness, N, V);
	
    lightAccumulatorAdd(lightAccumulator, nonSpecularContribution + specularContribution, nonSpecularContribution, u_AmbientCubemapColor, false);
    
    output.color = lightAccumulatorGetResult(lightAccumulator);
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