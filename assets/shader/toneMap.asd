<?xml version='1.0'?>

<effect>
    <parameter type="texture2D" name="u_SceneTex"/>
    <parameter type="texture1D" name="u_LuminanceTex"/>

    <macro name="RGB_TO_LUM" value="float3(0.2126f, 0.7152f, 0.0722f)"/>

    <parameter type="float2" name="u_Params"/>
    <parameter type="int" name="u_BlueShift"/>

    <parameter type="sampler" name="s_PointSampler">
        <state name="filtering" value="min_mag_mip_point"/>
        <state name="address_u" value="clamp"/>
        <state name="address_v" value="clamp"/>
        <state name="address_w" value="clamp"/>
    </parameter>
    
    <parameter type="sampler" name="s_LinearSampler">
        <state name="filtering" value="min_mag_linear_mip_point"/>
        <state name="address_u" value="clamp"/>
        <state name="address_v" value="clamp"/>
        <state name="address_w" value="clamp"/>
    </parameter>
    
    
    <shader>
        <![CDATA[
#include "toneMap.hlsl"
        ]]>
    </shader>

    <technique name="toneMap">
        <pass name="p0" domain="postProcess">
            <state name="cull_mode" value="none"/>
            <state name="depth_enable" value="false"/>
            <state name="depth_write_enable" value="false"/>
            <state name="pixel_shader" value="toneMapPS()"/>
            <state name="vertex_shader" value="tonemappingVS()"/>
        </pass>
    </technique>
</effect>
