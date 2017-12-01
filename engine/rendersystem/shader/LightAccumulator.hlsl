#ifndef _LightAccumulator_HLSL_
#define _LightAccumulator_HLSL_

#ifndef VISUALIZE_LIGHT_CULLING
#define VISUALIZE_LIGHT_CULLING 0
#endif

#define SUBSURFACE_CHANNEL_MODE 1

struct FLightAccumulator
{
	float3 TotalLight;
	float ScatterableLightLuma;
	float3 ScatterableLight;
	float EstimatedCost;
};

void LightAccumulator_Add(inout FLightAccumulator In, float3 TotalLight, float3 ScatterableLight, float3 CommonMultiplier, const bool bNeedsSeparateSubsurfaceLightAccumulation)
{
	In.TotalLight += TotalLight * CommonMultiplier;
}

float4 LightAccumulator_GetResult(FLightAccumulator In)
{
	float4 Ret;
	Ret = float4(In.TotalLight, 0);
	return Ret;
}


#endif