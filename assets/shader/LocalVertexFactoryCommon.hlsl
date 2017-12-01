#ifndef _LocalVertexFactoryCommon_HLSL_
#define _LocalVertexFactoryCommon_HLSL_

#if FEATURE_LEVEL >= FEATURE_LEVEL_ES3_1

struct FVertexFactoryInterpolantsVSToPS
{
#if NUM_MATERIAL_TEXCOORDS
	float4 TexCoords[(NUM_MATERIAL_TEXCOORDS + 1) / 2] : TEXCOORD0;
#elif USE_PARTICLE_SUBUVS
	float4 TexCoords[1] : TEXCOORD0;
#endif
};

#if NUM_MATERIAL_TEXCOORDS || USE_PARTICLE_SUBUVS
float2 GetUV(FVertexFactoryInterpolantsVSToPS Interpolants, int UVIndex)
{
	float4 UVVector = Interpolants.TexCoords[UVIndex / 2];
	return UVIndex % 2 ? UVVector.zw : UVVector.xy;
}
void SetUV(inout FVertexFactoryInterpolantsVSToPS Interpolants, int UVIndex, float2 InValue)
{
	FLATTEN
	if (UVIndex % 2)
	{
		Interpolants.TexCoords[UVIndex / 2].zw = InValue;
	}
	else
	{
		Interpolants.TexCoords[UVIndex / 2].xy = InValue;
	}
}
#endif

#else //#if FEATURE_LEVEL >= FEATURE_LEVEL_ES3_1

#define NUM_PACKED_UVS	(NUM_MATERIAL_TEXCOORDS > 1 ? 3 : 1)
#define USE_INTERPOLATOR_345 0

struct FVertexFactoryInterpolantsVSToPS
{
#if NUM_PACKED_UVS == 1
	float4 PackedUVs[1] : TEXCOORD0;
#if INTERPOLATE_VERTEX_COLOR
	float4 Color : TEXCOORD1;
#endif

#else
	float4 PackedUVs[3] : TEXCOORD0;

#endif
};

float2 GetUV(FVertexFactoryInterpolantsVSToPS Interpolants, int UVIndex)
{
	return Interpolants.PackedUVs[UVIndex].xy;
}
void SetUV(inout FVertexFactoryInterpolantsVSToPS Interpolants, int UVIndex, float2 InValue)
{
	Interpolants.PackedUVs[UVIndex].xy = InValue;
}
#endif //#if FEATURE_LEVEL >= FEATURE_LEVEL_ES3_1

#endif