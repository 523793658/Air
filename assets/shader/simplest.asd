<?xml version='1.0'?>

<effect>
    <parameter type="float4x4" name="mvp"/>
    <shader>
        <![CDATA[
void simplestVS(float4 g_Position : POSITION, out float4 v_Position : SV_Position)
{
	v_Position = mul(g_Position, mvp);
}
		
float4 simplestPS() : SV_Target
{
	return float4(1.0, 0.0, 0.0, 1.0);
}
		
        ]]>
    </shader>

    <technique name="simplest">
        <pass name="p0">
            <state name="cull_mode" value="none"/>
			<state name="polygon_mode" value="line"/>
            <state name="vertex_shader" value="simplestVS()"/>
            <state name="pixel_shader" value="simplestPS()"/>
        </pass>
    </technique>
</effect>
