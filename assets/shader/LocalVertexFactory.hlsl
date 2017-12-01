#include "VertexFactoryCommon.hlsl"
#include "LocalVertexFactoryCommon.hlsl"


FMaterialPixelParameters GetMaterialPixelParameters(FVertexFactoryInterpolantsVSToPS Interpolants, float4 SvPosition)
{
	FMaterialPixelParameters Result = MakeInitializedMaterialPixelParameters();

#if NUM_MATERIAL_TEXCOORDS
	UNROLL
	for (int CoordinateIndex = 0; CoordinateIndex < NUM_MATERIAL_TEXCOORDS; CoordinateIndex++)
	{
		Result.TexCoords[CoordinateIndex] = GetUV(Interpolants, CoordinateIndex);
	}
#endif
	return Result;
}