<?xml version='1.0'?>

<effect>
    <parameter type="float4x4" name="u_InvMVP" />
    <parameter type="float" name="u_DepthFar"/>
    <parameter type="int" name="u_SkyBoxCompressed"/>
    <parameter type="textureCUBE" name="u_SkyBoxTex"/>
    <parameter type="textureCUBE" name="u_SkyBoxCcubeTex"/>
    <parameter type="sampler" name="s_SkyboxSampler">
        <state name="filtering" value="min_mag_linear_mip_point"/>
        <state name="address_u" value="clamp"/>
        <state name="address_v" value="clamp"/>
        <state name="address_w" value="clamp"/>
    </parameter>
    <shader>
        <![CDATA[
#include "skyBox.hlsl"
        ]]>
    </shader>

    <technique name="skyBoxTech">
        <pass name="p0">
            <state name="cull_mode" value="none"/>
            <state name="depth_write_enable" value="0"/>
            <state name="depth_func" value="equal"/>
            <state name="vertex_shader" value="skyBoxVS()"/>
            <state name="pixel_shader" value="skyBoxPS()"/>
        </pass>
    </technique>
</effect>
