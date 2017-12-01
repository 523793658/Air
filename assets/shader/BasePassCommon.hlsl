


struct FSharedBasePassInterpolants
{
#if NEEDS_BASEPASS_FOGGING
	float4 VertexFog		:TEXCOORD7;
#endif
#if !0
#if USE_WORLD_POSITION_EXCLUDING_SHADER_OFFSETS
	float3 PixelPositionExcludingWPO : TEXCOORD9;
#endif
#endif

#if TRANSLUCENCY_PERVERTEX_LIGHTING_VOLUME
	float3 AmbientLightingVector : TEXCOORD12;
#endif

#if TRANSLUCENCY_PERVERTEX_LIGHTING_VOLUME && TRANSLUCENCY_LIGHTING_VOLUMETRIC_PERVERTEX_DIRECTIONAL
	float3 DirectionalLightingVector : TEXCOORD13;
#endif

#if TRANSLUCENCY_PERVERTEX_FORWARD_SHADING
	float3 VertexDiffuseLighting : TEXCOORD12;
#endif

#if WRITES_VELOCITY_TO_GBUFFER
	// .xy is clip position, pre divide by w; .w is clip W; .z is 0 or 1 to mask out the velocity output
	float4 VelocityPrevScreenPosition : VELOCITY_PREV_POS;
#if WRITES_VELOCITY_TO_GBUFFER_USE_POS_INTERPOLATOR
	float4 VelocityScreenPosition : VELOCITY_POS;
#endif
#endif
};


#if PC_D3D || GL4_PROFILE || SWITCH_PROFILE
struct FBasePassInterpolantsVSToPS : FSharedBasePassInterpolants
{
#if USE_WORLD_POSITION_EXCLUDING_SHADER_OFFSETS
	float3 PixelPositionExcludingWPO : TEXCOORD9;
#endif
}

struct FBasePassInterpolantsVSToDS : FSharedBasePassInterpolants
{
#if USE_WORLD_POSITION_EXCLUDING_SHADER_OFFSETS
	float3 WorldPositionExcludingWPO : TEXCOORD9;
#endif
};
#else
#define FBasePassInterpolantsVSToPS FSharedBasePassInterpolants
#endif