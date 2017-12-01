#ifndef _Definitions_HLSL_
#define _Definitions_HLSL_


#ifndef SM5_PROFILE
#define SM5_PROFILE			0
#endif

#ifndef SM4_PROFILE
#define SM4_PROFILE			0
#endif

#ifndef ES2_PROFILE
#define ES2_PROFILE			0
#endif

#ifndef ES3_1_PROFILE
#define ES3_1_PROFILE		0
#endif

#ifndef METAL_MRT_PROFILE
#define METAL_MRT_PROFILE		0
#endif

#ifndef METAL_SM4_PROFILE
#define METAL_SM4_PROFILE		0
#endif

#ifndef METAL_SM5_PROFILE
#define METAL_SM5_PROFILE		0
#endif

#ifndef VULKAN_PROFILE_SM4
#define VULKAN_PROFILE_SM4		0
#endif

#ifndef VULKAN_PROFILE_SM5
#define VULKAN_PROFILE_SM5		0
#endif

#ifndef IOS
#define IOS						0
#endif

#ifndef MAC
#define MAC						0
#endif

#ifndef USING_TESSELLATION
#define USING_TESSELLATION		0
#endif

#ifndef GBUFFER_HAS_VELOCITY
#define GBUFFER_HAS_VELOCITY	0
#endif

#define PC_D3D						SM5_PROFILE

#ifndef NEEDS_LIGHTMAP_COORDINATE
#define NEEDS_LIGHTMAP_COORDINATE	0
#endif


#if SM5_PROFILE || COMPILER_SUPPORTS_ATTRIBUTES

#define UNROLL					[unroll]
#define LOOP					[loop]
#define BRANCH					[branch]
#define FLATTEN					[flatten]
#define ALLOW_UAV_CONDITION		[allow_uav_condition]

#else

#ifndef UNROLL
#define UNROLL
#endif

#ifndef LOOP
#define LOOP
#endif

#ifndef BRANCH
#define BRANCH
#endif

#ifndef FLATTEN
#define FLATTEN
#endif

#ifndef ALLOW_UAV_CONDITION
#define ALLOW_UAV_CONDITION
#endif
#endif


#endif