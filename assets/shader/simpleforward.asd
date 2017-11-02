<?xml version='1.0'?>

<effect>
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


#include "shadingModels.hlsl"

struct PS_INPUT
{
    float4 oPos : SV_Position;
	float3 vNormal : NORMAL0;
};


struct PS_OUTPUT
{
	float4 color : SV_Target;
};


PS_OUTPUT simplestPS(PS_INPUT input)
{
	PS_OUTPUT output;
    
#ifdef STANDARDSURFACE
    SurfaceOutputStandard params = (SurfaceOutputStandard)0;
    params.roughness = u_BaseColorRoughness.w;
    params.metallic = u_SpecularColorMetallic.w;
    params.normal = input.vNormal;
	params.localSpecular = u_SpecularColorMetallic.rgb;
    params.baseColor = u_BaseColorRoughness.rgb;
    output.color = standardSurfaceShading(params);
#else
    output.color = float4(1.0, 0.0, 0.0, 1.0);
#endif
    return output;
}
		]]>
	</shader>
	<technique name="simplest">
		<pass name="p0" domain="surface" blendMode="opaque" shadingModel="defaultLit">
			<state name = "pixel_shader" value="simplestPS()"/>
			<state name = "cull_mode" value="back" />
		</pass>
	</technique>
</effect>