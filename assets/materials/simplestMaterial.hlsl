

//材质中的Uniform参数
cbuffer MaterialUniformBuffer
{
	float4 color;
};




struct FMaterialPixelParameters
{
#if NUM_MATERIAL_TEXCOORDS
	float2 TexCoords[NUM_MATERIAL_TEXCOORDS];
#endif

	half4 VertexColor;
	half3 WorldNormal;
	half3 ReflectionVector;
	half3 CameraVector;
	half3 LightVector;

	float4 vPosition;
	float4 ScreenPosition;
	half UnMirrored;
	half TwoSidedSign;

	half3x3	TangentToWorld;
	float3 AbsoluteWorldPosition;
	float3 WorldPosition_CamRelative;
};

FMaterialPixelParameters MakeInitializedMaterialPixelParameters()
{
	FMaterialPixelParameters MPP;
	MPP = (FMaterialPixelParameters)0;
	MPP.TangentToWorld = float3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
	return MPP;
}


//由材质来填充结构体的内容，目前写死。
struct FPixelMaterialInputs
{
	half3 BaseColor;
	half Metallic;
	half Specular;
	half Roughness;
	half AmbientOcclusion;
	half3 EmissiveColor;
	half3 Normal;
};

half3 GetMaterialBaseColorRaw(FPixelMaterialInputs PixelMaterialInputs)
{
	return PixelMaterialInputs.BaseColor;
}

half3 GetMaterialBaseColor(FPixelMaterialInputs PixelMaterialInputs)
{
	return saturate(GetMaterialBaseColorRaw(PixelMaterialInputs));
}

half GetMaterialMetallicRaw(FPixelMaterialInputs PixelMaterialInputs)
{
	return PixelMaterialInputs.Metallic;
}

half GetMaterialMetallic(FPixelMaterialInputs PixelMaterialInputs)
{
	return saturate(GetMaterialMetallicRaw(PixelMaterialInputs));
}

half GetMaterialSpecularRaw(FPixelMaterialInputs PixelMaterialInputs)
{
	return PixelMaterialInputs.Specular;
}

half GetMaterialSpecular(FPixelMaterialInputs PixelMaterialInputs)
{
	return saturate(GetMaterialSpecularRaw(PixelMaterialInputs));
}

half GetMaterialRoughnessRaw(FPixelMaterialInputs PixelMaterialInputs)
{
	return PixelMaterialInputs.Roughness;
}

half GetMaterialRoughness(FPixelMaterialInputs PixelMaterialInputs)
{
#if MATERIAL_FULLY_ROUGH
	return 1;
#endif
#if (ES2_PROFILE || ES3_1_PROFILE)
	half Roughness = GetMaterialRoughnessRaw(PixelMaterialInputs);
	return clamp(Roughness, 0.12, 1.0);
#else
	half Roughness = saturate(GetMaterialRoughnessRaw(PixelMaterialInputs));
	return Roughness;
#endif
}

half3 GetMaterialEmissiveRaw(FPixelMaterialInputs PixelMaterialInputs)
{
	return PixelMaterialInputs.EmissiveColor;
}

half3 GetMaterialEmissive(FPixelMaterialInputs PixelMaterialInputs)
{
	half3 EmissiveColor = GetMaterialEmissiveRaw(PixelMaterialInputs);
#if !MATERIAL_ALLOW_NEGATIVE_EMISSIVECOLOR
	EmissiveColor = max(EmissiveColor, 0.0f);
#endif
	return EmissiveColor;
}


half GetMaterialAmbientOcclusionRaw(FPixelMaterialInputs PixelMaterialInputs)
{
	return PixelMaterialInputs.AmbientOcclusion;
}

half GetMaterialAmbientOcclusion(FPixelMaterialInputs PixelMaterialInputs)
{
	return saturate(GetMaterialAmbientOcclusionRaw(PixelMaterialInputs));
}

half3 GetMaterialNormalRaw(FPixelMaterialInputs PixelMaterialInputs)
{
	return PixelMaterialInputs.Normal;
}

half3 GetMaterialNormal(FMaterialPixelParameters Parameters, FPixelMaterialInputs PixelMaterialInputs)
{
	half3 RetNormal;

	RetNormal = GetMaterialNormalRaw(PixelMaterialInputs);

#if (USE_EDITOR_SHADERS && !(ES2_PROFILE || ES3_1_PROFILE || ESDEFERRED_PROFILE)) || MOBILE_EMULATION
	{
		// this feature is only needed for development/editor - we can compile it out for a shipping build (see r.CompileShadersForDevelopment cvar help)
		half3 OverrideNormal = View.NormalOverrideParameter.xyz;

#if !MATERIAL_TANGENTSPACENORMAL
		OverrideNormal = Parameters.TangentToWorld[2] * (1 - View.NormalOverrideParameter.w);
#endif

		RetNormal = RetNormal * View.NormalOverrideParameter.w + OverrideNormal;
	}
#endif

	return RetNormal;
}


#if COMPILER_HLSL

#define INPUT_POSITION_QUALIFIERS linear noperspective centroid
#define DEPTH_WRITE_SEMANTIC SV_DepthLessEqual

#else
#error USE_CONSERVATIVE_DEPTH_WRITES enabled for unsupported platform

#endif

#define FrontFaceSementic SV_IsFrontFace
#define FIsFrontFace bool


#if MATERIAL_TWOSIDED_SEPARATE_PASS
#define OPTIONAL_IsFrontFace 
static const FIsFrontFace bIsFrontFace = 1;
#else
#define OPTIONAL_IsFrontFace , in FIsFrontFace bIsFrontFace : FrontFaceSementic
#endif


#if OUTPUT_PIXEL_DEPTH_OFFSET
#define OPTIONAL_OutDepthConservative ,out float OutDepth : DEPTH_WRITE_SEMANTIC
#define OPTIONAL_OutDepth ,out float OutDepth : SV_DEPTH
#else
#define OPTIONAL_OutDepthConservative
#define OPTIONAL_OutDepth
#endif


void CalcPixelMaterialInputs(in out FMaterialPixelParameters Parameters, in out FPixelMaterialInputs PixelMaterialInputs)
{
	float3 MaterialNormal = GetMaterialNormal(Parameters, PixelMaterialInputs);
#if MATERIAL_TANGENTSPACENORMAL
#if FEATURE_LEVEL >= FEATURE_LEVEL_SM4
	MaterialNormal = normalize(MaterialNormal);
#endif
	Parameters.WorldNormal = TransformTangentNormalToWorld(Parameters, MaterialNormal);
#else
	Parameters.WorldNormal = normalize(MaterialNormal);
#endif

#if MATERIAL_TANGENTSPACENORMAL
	Parameters.WorldNormal *= Parameters.TwoSidedSign;
#endif
	//Parameters.ReflectionVector = RefectionAboutCustomWorldNormal(Parameters, Parameters.WorldNormal, false);

#if !PARTICLE_SPRITE_FACTORY
#endif // !PARTICLE_SPRITE_FACTORY
	//材质定义部分，这里直接复制，后续可以在材质中编辑
	PixelMaterialInputs.BaseColor = float3(1, 1, 1);
	PixelMaterialInputs.Metallic = 1.0;
	PixelMaterialInputs.Specular = 1.0;
	PixelMaterialInputs.Roughness = 0.0;
	PixelMaterialInputs.AmbientOcclusion = 1.0;
	PixelMaterialInputs.EmissiveColor = float3(0, 0, 0);
	PixelMaterialInputs.Normal = Parameters.WorldNormal;
}


void CalcMaterialParametersEx(in out FMaterialPixelParameters Parameters,
	in out FPixelMaterialInputs PixelMaterialInputs,
	float4 vPosition,
	float4 ScreenPosition,
	FIsFrontFace bIsFrontFace,
	float3 TranslatedWorldPosition,
	float3 TranslatedWorldPositionExcludingShaderOffsets)
{
	Parameters.WorldPosition_CamRelative = TranslatedWorldPosition.xyz;
	Parameters.AbsoluteWorldPosition = TranslatedWorldPosition.xyz - ResolvedView.PreViewTranslation.xyz;

	// If the material uses any non-offset world position expressions, calculate those parameters. If not, 
	// the variables will have been initialised to 0 earlier.
#if USE_WORLD_POSITION_EXCLUDING_SHADER_OFFSETS
	Parameters.WorldPosition_NoOffsets_CamRelative = TranslatedWorldPositionExcludingShaderOffsets;
	Parameters.WorldPosition_NoOffsets = TranslatedWorldPositionExcludingShaderOffsets - ResolvedView.PreViewTranslation.xyz;
#endif

	Parameters.vPosition = vPosition;
	Parameters.ScreenPosition = ScreenPosition;

#if COMPILER_GLSL_ES2 || COMPILER_GLSL_ES3_1
	// ES2 normalize isn't done accurately. This seems to fix it.
	// Originally this was normalize(normalize(TranslatedWorldPosition.xyz)) but tegra4 appears to optimize that out.
	Parameters.CameraVector = normalize(-0.01 * Parameters.WorldPosition_CamRelative.xyz);
#else
	// TranslatedWorldPosition is the world position translated to the camera position, which is just -CameraVector
	Parameters.CameraVector = normalize(-Parameters.WorldPosition_CamRelative.xyz);
#endif

	Parameters.LightVector = 0;

	Parameters.TwoSidedSign = 1.0f;

#if MATERIAL_TWOSIDED && HAS_PRIMITIVE_UNIFORM_BUFFER
	Parameters.TwoSidedSign *= View.CullingSign * Primitive.LocalToWorldDeterminantSign;

#if !MATERIAL_TWOSIDED_SEPARATE_PASS
	Parameters.TwoSidedSign *= GetFloatFacingSign(bIsFrontFace);
#endif
#endif

	// Now that we have all the pixel-related parameters setup, calculate the Material Input/Attributes and Normal
	CalcPixelMaterialInputs(Parameters, PixelMaterialInputs);
}
