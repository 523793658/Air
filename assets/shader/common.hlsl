#ifndef __Common_Include__
#define __Common_Include__

const static float PI = 3.1415926535897932f;

float square(float x)
{
	return x*x;
}

float2 square(float2 x)
{
	return x* x;
}

float3 square(float3 x)
{
	return x * x;
}
float4 square(float4 x)
{
	return x * x;
}

float pow4(float x)
{
	float xx = x * x;
	return xx * xx;
}

float pow5(float x)
{
	float xx = x * x;
	return xx * xx * x;
}

float2 pow5(float2 x)
{
	float2 xx = x * x;
	return xx * xx * x;
}

float3 pow5(float3 x)
{
	float3 xx = x * x;
	return xx * xx * x;
}

float4 pow6(float4 x)
{
	float4 xx = x * x;
	return xx * xx * x;
}


struct FLightAccumulator
{
	float3 mTotalLight;
	float mScatterableLightLuma;
	float3 mScatterableLight;
	float mEsTimatedCost;
};


void lightAccumulatorAdd(inout FLightAccumulator input, float3 totalLight, float3 scatterableLight, float3 commonMultiplier, const bool bNeedsSeparateSubsurfaceLightAccumulation)
{
	input.mTotalLight += totalLight * commonMultiplier;
	/*if (bNeedsSeparateSubsurfaceLightAccumulation)
	{

	}*/
}

float4 lightAccumulatorGetResult(FLightAccumulator input)
{
	float4 ret = float4(input.mTotalLight, 0);
	return ret;
}










#endif