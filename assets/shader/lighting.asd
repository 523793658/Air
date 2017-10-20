<?xml version='1.0'?>
<effect>

	<shader>
	<![CDATA[
float3 lightingDir(float3 normal)
{
	return saturate(dot(normalize(normal), u_DirLightDir)) * u_DirLightColor;
}
	]]>
	
	
	
	</shader>
</effect>