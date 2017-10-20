#ifndef __BRDF_COMMON__
#define __BRDF_COMMON__

float D_GGX(float roughness, float NoH)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float d = (NoH * a2 - NoH) * NoH + 1;
	return a2 / (PI * d * d);
}


float Vis_SmithJointApprox(float roughness, float NoV, float NoL)
{
	float a = square(roughness);
	float Vis_SmithV = NoL * (NoV * (1 - a) + a);
	float Vis_SmithL = NoV * (NoL * (1 - a) + a);
	return 0.5 * rcp(Vis_SmithV + Vis_SmithL);
}

float3 F_Schlick(float3 specularColor, float VoH)
{
	float Fc = pow5(1 - VoH);
	return saturate(50.0 * specularColor.g) * Fc + (1 - Fc) * specularColor;
}

float3 diffuse_lambert(float3 diffuseColor)
{
	return diffuseColor * (1 / PI);
}
#endif
