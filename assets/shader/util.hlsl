#ifndef __Util_Include__
#define __Util_Include__

float2 get_xy_channel(float4 v)
{
#ifdef AIR_BC5_AS_AG
	return v.ag;
#else
#ifdef AIR_BC5_AS_GA
	return v.ga;
#else
	return v.rg;
#endif
#endif
}

float4 decode_hdr_yc(float y, float4 c)
{
	float Y = exp2(y * 65536 / 2048 - 16);
	float2 C = get_xy_channel(c);
	C *= C;
	return float4(Y * float3(C.g, (1.0f - C.g - C.r), C.r) / float3(0.2126f, 0.7152f, 0.0722f), 1);
}

#endif