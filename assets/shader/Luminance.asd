<?xml version='1.0'?>

<effect>
	<parameter type="uint4" name="u_LuminanceParams" />

    <parameter type="texture2D" name="u_SceneColorTex"/>
    <parameter type="sampler" name="s_SceneTexSampler">
        <state name="filtering" value="min_mag_linear_mip_point"/>
        <state name="address_u" value="clamp"/>
        <state name="address_v" value="clamp"/>
        <state name="address_w" value="clamp"/>
    </parameter>
	
	<parameter type="rw_texture2D" name= "u_Lum2DTexOutput"/>
	<parameter type="rw_texture1D" name= "u_LuminanceTex"/>
	
    <shader>
        <![CDATA[
#include "Luminance.hlsl"
        ]]>
    </shader>

    <technique name="luminanceCS">
        <pass name="p0" hasVertexShader = "false">
			<macro name="REDUCETO1D" value="1"/>
            <state name="cull_mode" value="none"/>
            <state name="depth_enable" value="false"/>
            <state name="depth_write_enable" value="false"/>
            <state name="compute_shader" value="reduceTo1DCS()"/>
            
        </pass>
		
		<pass name="p1" hasVertexShader="false">
			<macro name="REDUCETOSINGLE" value="1"/>
			<state name="compute_shader" value="reduceToSingleCS()" dispatch="int3(1, 1, 1)"/>
		</pass>
    </technique>
</effect>
