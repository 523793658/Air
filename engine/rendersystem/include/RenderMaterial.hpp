#ifndef _RenderMaterial_H_
#define _RenderMaterial_H_
namespace Air
{
	enum LightingTechnique
	{
		LT_Forward,
		LT_Deferred
	};


	class AIR_CORE_API RenderMaterial
	{
	public:
		RenderMaterial();
		RenderMaterial(float3 const & diff, float3 const &spec, float glossiness, float metallic);
	public:
		enum TextureSlot
		{
			TS_Albedo,
			TS_Metalness,
			TS_Glossiness,
			TS_Emissive,
			TS_Normal,
			TS_Height,
			TS_NumTextureSlots
		};
		enum SurfaceDetailMode
		{
			SDM_Parallax = 0,
			SDM_FlatTessellation,
			SDM_SmoothTessellation
		};

		std::string mName;
		float4 mAlbedoRoughness;
		float4 mSpecularMetalness;
		float3 mEmissive;

		bool mTransparent;
		float mAlphaTest;
		bool mSSS;
		std::array<std::string, TS_NumTextureSlots> mTextureNames;
		SurfaceDetailMode mDetailMode;
		float2 mHeightOffsetScale;
		float4 mTessFactors;
	};

	float const MAX_SHININESS = 8192;
	float const INV_LOG_MAX_SHININESS = 1 / log(MAX_SHININESS);
	inline float shininess2Glossiness(float shininess)
	{
		return log(shininess) * INV_LOG_MAX_SHININESS;
	}

	inline float glossiness2Shininess(float glossiness)
	{
		return pow(MAX_SHININESS, glossiness);
	}

	AIR_CORE_API RenderMaterialPtr syncLoadRenderMaterial(std::string const & mtlml_name);
	AIR_CORE_API RenderMaterialPtr aSyncLoadRenderMaterial(std::string const & mtlml_name);
	AIR_CORE_API void saveRenderMaterial(RenderMaterialPtr const & mtl, std::string const & mtlml_name);
}


#endif