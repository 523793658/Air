#ifndef _Renderable_H_
#define _Renderable_H_
#pragma once

namespace Air
{


	enum PassCategory
	{
		PC_Depth = 0,
		PC_GBuffer,
		PC_ShadowMap,
		PC_Shadowing,
		PC_IndirectLighting,
		PC_Shading,
		PC_Reflection,
		PC_SpecialShading,
		PC_SimpleForward,
		PC_None
	};

	enum PassTargetBuffer
	{
		PTB_Opaque = 0,
		PTB_TransparencyBack,
		PTB_TransparencyFront,
		PTB_None
	};

	enum PassRT
	{
		PRT_RT0 = 0,
		PRT_RT1,
		PRT_MRT,
		PRT_ShadowMap,
		PRT_ShadowMapWODepth,
		PRT_CascadedShadowMap,
		PRT_ReflectiveShadowMap,
		PRT_None
	};

	// pass type is a 32-bit value:
	// 0000000000000000000000 C3 C2 C1 C0 TB1 TB0 RT3 RT2 RT1 RT0

	template <PassRT rt, PassTargetBuffer tb, PassCategory cat>
	struct MakePassType
	{
		static uint32_t const value = (rt << 0) | (tb << 4) | (cat << 6);
	};

	enum PassType
	{
		PT_OpaqueDepth = MakePassType<PRT_None, PTB_Opaque, PC_Depth>::value,
		PT_TransparencyBackDepth = MakePassType<PRT_None, PTB_TransparencyBack, PC_Depth>::value,
		PT_TransparencyFrontDepth = MakePassType<PRT_None, PTB_TransparencyFront, PC_Depth>::value,

		PT_OpaqueGBufferRT0 = MakePassType<PRT_RT0, PTB_Opaque, PC_GBuffer>::value,
		PT_TransparencyBackGBufferRT0 = MakePassType<PRT_RT0, PTB_TransparencyBack, PC_GBuffer>::value,
		PT_TransparencyFrontGBufferRT0 = MakePassType<PRT_RT0, PTB_TransparencyFront, PC_GBuffer>::value,
		PT_OpaqueGBufferRT1 = MakePassType<PRT_RT1, PTB_Opaque, PC_GBuffer>::value,
		PT_TransparencyBackGBufferRT1 = MakePassType<PRT_RT1, PTB_TransparencyBack, PC_GBuffer>::value,
		PT_TransparencyFrontGBufferRT1 = MakePassType<PRT_RT1, PTB_TransparencyFront, PC_GBuffer>::value,
		PT_OpaqueGBufferMRT = MakePassType<PRT_MRT, PTB_Opaque, PC_GBuffer>::value,
		PT_TransparencyBackGBufferMRT = MakePassType<PRT_MRT, PTB_TransparencyBack, PC_GBuffer>::value,
		PT_TransparencyFrontGBufferMRT = MakePassType<PRT_MRT, PTB_TransparencyFront, PC_GBuffer>::value,

		PT_GenShadowMap = MakePassType<PRT_ShadowMap, PTB_None, PC_ShadowMap>::value,
		PT_GenShadowMapWODepthTexture = MakePassType<PRT_ShadowMapWODepth, PTB_None, PC_ShadowMap>::value,
		PT_GenCascadedShadowMap = MakePassType<PRT_CascadedShadowMap, PTB_None, PC_ShadowMap>::value,
		PT_GenReflectiveShadowMap = MakePassType<PRT_ReflectiveShadowMap, PTB_None, PC_ShadowMap>::value,

		PT_Shadowing = MakePassType<PRT_None, PTB_None, PC_Shadowing>::value,

		PT_IndirectLighting = MakePassType<PRT_None, PTB_None, PC_IndirectLighting>::value,

		PT_OpaqueShading = MakePassType<PRT_None, PTB_Opaque, PC_Shading>::value,
		PT_TransparencyBackShading = MakePassType<PRT_None, PTB_TransparencyBack, PC_Shading>::value,
		PT_TransparencyFrontShading = MakePassType<PRT_None, PTB_TransparencyFront, PC_Shading>::value,

		PT_OpaqueReflection = MakePassType<PRT_None, PTB_Opaque, PC_Reflection>::value,
		PT_TransparencyBackReflection = MakePassType<PRT_None, PTB_TransparencyBack, PC_Reflection>::value,
		PT_TransparencyFrontReflection = MakePassType<PRT_None, PTB_TransparencyFront, PC_Reflection>::value,

		PT_OpaqueSpecialShading = MakePassType<PRT_None, PTB_Opaque, PC_SpecialShading>::value,
		PT_TransparencyBackSpecialShading = MakePassType<PRT_None, PTB_TransparencyBack, PC_SpecialShading>::value,
		PT_TransparencyFrontSpecialShading = MakePassType<PRT_None, PTB_TransparencyFront, PC_SpecialShading>::value,

		PT_SimpleForward = MakePassType<PRT_None, PTB_None, PC_SimpleForward>::value
	};

	inline PassRT GetPassRT(PassType pt)
	{
		return static_cast<PassRT>(pt & 0xF);
	}
	inline PassTargetBuffer GetPassTargetBuffer(PassType pt)
	{
		return static_cast<PassTargetBuffer>((pt >> 4) & 0x3);
	}
	inline PassCategory GetPassCategory(PassType pt)
	{
		return static_cast<PassCategory>((pt >> 6) & 0xF);
	}
	inline PassType ComposePassType(PassRT rt, PassTargetBuffer tb, PassCategory cat)
	{
		return static_cast<PassType>((rt << 0) | (tb << 4) | (cat << 6));
	}

	class AIR_CORE_API Renderable
	{
	public:
		enum EffectAttribute
		{
			EA_SpecialShading = 1UL << 0,
			EA_TransparencyBack = 1UL << 1,
			EA_TransparencyFront = 1UL << 2,
			EA_AlphaTest = 1UL << 3,
			EA_Reflection = 1UL << 4,
			EA_SimpleForward = 1UL << 5,
			EA_SSS = 1UL << 6
		};

	public:
		Renderable();
		virtual ~Renderable();

		virtual bool getHWResourceReady() const;
	};
}



#endif