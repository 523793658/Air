#ifndef _PixelShaderOutputCommon_H_
#define _PixelShaderOutputCommon_H_

#ifndef PIXELSHADEROUTPUT_INTERPOLANTS
	#define PIXELSHADEROUTPUT_INTERPOLANTS 0
#endif

#ifndef PIXELSHADEROUTPUT_BASEPASS
	#define PIXELSHADEROUTPUT_BASEPASS 0
#endif

#ifndef PIXELSHADEROUTPUT_MRT0
	#define PIXELSHADEROUTPUT_MRT0 0
#endif

#ifndef PIXELSHADEROUTPUT_MRT1
	#define PIXELSHADEROUTPUT_MRT1 0
#endif

#ifndef PIXELSHADEROUTPUT_MRT2
	#define PIXELSHADEROUTPUT_MRT2 0
#endif

#ifndef PIXELSHADEROUTPUT_MRT3
	#define PIXELSHADEROUTPUT_MRT3 0
#endif

#ifndef PIXELSHADEROUTPUT_MRT4
	#define PIXELSHADEROUTPUT_MRT4 0
#endif

#ifndef PIXELSHADEROUTPUT_MRT5
	#define PIXELSHADEROUTPUT_MRT5 0
#endif

#ifndef PIXELSHADEROUTPUT_MRT6
	#define PIXELSHADEROUTPUT_MRT6 0
#endif

#ifndef PIXELSHADEROUTPUT_MRT7
	#define PIXELSHADEROUTPUT_MRT7 0
#endif
void MainPS
(
#if PIXELSHADEROUTPUT_BASEPASS || PIXELSHADEROUTPUT_INTERPOLANTS
	FVertexFactoryInterpolantsVSToPS Interpolants,
#endif
#if PIXELSHADEROUTPUT_BASEPASS
	FBasePassInterpolantsVSToPS BasePassInterpolants,
#endif

	in INPUT_POSITION_QUALIFIERS float4 vPosition : SV_Position

	OPTIONAL_IsFrontFace

#ifdef PIXELSHADEROUTPUT_MRT0
	, out float4 OutTarget0 : SV_Target0
#endif

#if PIXELSHADEROUTPUT_MRT1
	, out float4 OutTarget1 : SV_Target1
#endif

#if PIXELSHADEROUTPUT_MRT2
	, out float4 OutTarget2 : SV_Target2
#endif

#if PIXELSHADEROUTPUT_MRT3
	, out float4 OutTarget3 : SV_Target3
#endif

#if PIXELSHADEROUTPUT_MRT4
	, out float4 OutTarget4 : SV_Target4
#endif

#if PIXELSHADEROUTPUT_MRT5
	, out float4 OutTarget5 : SV_Target5
#endif

#if PIXELSHADEROUTPUT_MRT6
	, out float4 OutTarget6 : SV_Target6
#endif

#if PIXELSHADEROUTPUT_MRT7
	, out float4 OutTarget7 : SV_Target7
#endif

	OPTIONAL_OutDepthConservative
)
{
	FPixelShaderIn PixelShaderIn = (FPixelShaderIn)0;
	FPixelShaderOut PixelShaderOut = (FPixelShaderOut)0;
	PixelShaderIn.Coverage = 0xF;
	PixelShaderOut.Coverage = PixelShaderIn.Coverage;
	PixelShaderIn.vPosition = vPosition;
	PixelShaderIn.bIsFrontFace = bIsFrontFace;
#if PIXELSHADEROUTPUT_BASEPASS
	FPixelShaderInOut_MainPS(Interpolants, BasePassInterpolants, PixelShaderIn, PixelShaderOut);
#else
#endif

#if PIXELSHADEROUTPUT_MRT0
	OutTarget0 = PixelShaderOut.MRT[0];
#endif

#if PIXELSHADEROUTPUT_MRT1
	OutTarget1 = PixelShaderOut.MRT[1];
#endif

#if PIXELSHADEROUTPUT_MRT2
	OutTarget2 = PixelShaderOut.MRT[2];
#endif

#if PIXELSHADEROUTPUT_MRT3
	OutTarget3 = PixelShaderOut.MRT[3];
#endif

#if PIXELSHADEROUTPUT_MRT4
	OutTarget4 = PixelShaderOut.MRT[4];
#endif

#if PIXELSHADEROUTPUT_MRT5
	OutTarget5 = PixelShaderOut.MRT[5];
#endif

#if PIXELSHADEROUTPUT_MRT6
	OutTarget6 = PixelShaderOut.MRT[6];
#endif

#if PIXELSHADEROUTPUT_MRT7
	OutTarget7 = PixelShaderOut.MRT[7];
#endif

#if OUTPUT_PIXEL_DEPTH_OFFSET
	OutDepth = PixelShaderOut.Depth;
#endif 
}



















#endif