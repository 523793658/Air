#ifndef _Luminance_H_
#define _Luminance_H_




#define BlockSizeX 8
#define BlockSizeY 8
#define GroupCount 16

#ifdef REDUCETO1D
#define groupThreads (BlockSizeX * BlockSizeY)

groupshared float accum[groupThreads];

static const float4 LUM_VECTOR = float4(.299, .587, .114, 0);

[numthreads(BlockSizeX, BlockSizeY, 1)]
void reduceTo1DCS(uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex)
{
	uint2 offset = DTid.xy * 4;
	float2 texCoord0 = (offset + float2(0.5, 0.5)) / u_LuminanceParams.xy;
	float2 texCoord1 = (offset + float2(2.5, 0.5)) / u_LuminanceParams.xy;
	float2 texCoord2 = (offset + float2(0.5, 2.5)) / u_LuminanceParams.xy;
	float2 texCoord3 = (offset + float2(2.5, 2.5)) / u_LuminanceParams.xy;
	accum[GI] = dot(u_SceneColorTex.SampleLevel(s_SceneTexSampler, texCoord0, 0), LUM_VECTOR) +
		dot(u_SceneColorTex.SampleLevel(s_SceneTexSampler, texCoord1, 0), LUM_VECTOR) +
		dot(u_SceneColorTex.SampleLevel(s_SceneTexSampler, texCoord2, 0), LUM_VECTOR) +
		dot(u_SceneColorTex.SampleLevel(s_SceneTexSampler, texCoord3, 0), LUM_VECTOR);
	GroupMemoryBarrierWithGroupSync();

	if (GI < 32)
	{
		accum[GI] += accum[GI + 32];
	}
	GroupMemoryBarrierWithGroupSync();

	if (GI < 16)
	{
		accum[GI] += accum[GI + 16];
	}

	GroupMemoryBarrierWithGroupSync();
	if (GI < 8)
	{
		accum[GI] += accum[GI + 8];
	}

	GroupMemoryBarrierWithGroupSync();
	if (GI < 4)
	{
		accum[GI] += accum[GI + 4];
	}

	GroupMemoryBarrierWithGroupSync();
	if (GI < 2)
	{
		accum[GI] += accum[GI + 2];
	}

	GroupMemoryBarrierWithGroupSync();

	if (GI < 1)
	{
		accum[GI] += accum[GI + 1];
		u_Lum2DTexOutput[Gid.xy] = accum[GI] / 256;
	}
}
#endif

#ifdef REDUCETOSINGLE
groupshared float luminance[GroupCount * GroupCount];
[numthreads(GroupCount, GroupCount, 1)]
void reduceToSingleCS(uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex)
{
	luminance[GI] = u_Lum2DTexOutput[DTid.xy].r;


	GroupMemoryBarrierWithGroupSync();

	if (GI < 128)
	{
		luminance[GI] += luminance[GI + 128];
	}


	GroupMemoryBarrierWithGroupSync();

	if (GI < 64)
	{
		luminance[GI] += luminance[GI + 64];
	}

	GroupMemoryBarrierWithGroupSync();

	if (GI < 32)
	{
		luminance[GI] += luminance[GI + 32];
	}
	GroupMemoryBarrierWithGroupSync();

	if (GI < 16)
	{
		luminance[GI] += luminance[GI + 16];
	}

	GroupMemoryBarrierWithGroupSync();
	if (GI < 8)
	{
		luminance[GI] += luminance[GI + 8];
	}

	GroupMemoryBarrierWithGroupSync();
	if (GI < 4)
	{
		luminance[GI] += luminance[GI + 4];
	}

	GroupMemoryBarrierWithGroupSync();
	if (GI < 2)
	{
		luminance[GI] += luminance[GI + 2];
	}

	GroupMemoryBarrierWithGroupSync();

	if (GI < 1)
	{
		luminance[GI] += luminance[GI + 1];
		u_LuminanceTex[0] = luminance[GI] / 256;
	}
}

#endif
#endif