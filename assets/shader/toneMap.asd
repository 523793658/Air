<?xml version='1.0'?>

<effect>
    <parameter type="texture2D" name="u_SceneTex"/>
    <parameter type="sampler" name="s_SceneTexSampler">
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
        <pass name="p0">
            <state name="cull_mode" value="none"/>
            <state name="depth_enable" value="false"/>
            <state name="depth_write_enable" value="false"/>
            <state name="vertex_shader" value="toneMapVS()"/>
            <state name="pixel_shader" value="toneMapPS()"/>
        </pass>
    </technique>
</effect>
