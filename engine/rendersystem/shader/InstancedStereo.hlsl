#ifndef _InstancedStereo_HLSL_
#define _InstancedStereo_HLSL_

cbuffer ViewUniformBuffer
{
	float4 ViewRectMin;
	float4 ViewSizeAndInvSize;
	float4x4 SVPositionToTranslatedWorld;
	float4 PreViewTranslation;
};

struct ViewState
{
	float4 ViewRectMin;
	float4 ViewSizeAndInvSize;
	float4x4 SVPositionToTranslatedWorld;
	float4 PreViewTranslation;
};

ViewState GetPrimaryView()
{
	ViewState Result;
	Result.ViewRectMin = ViewRectMin;
	Result.ViewSizeAndInvSize = ViewSizeAndInvSize;
	Result.SVPositionToTranslatedWorld = SVPositionToTranslatedWorld;
	Result.PreViewTranslation = PreViewTranslation;
	return Result;
}








static ViewState ResolvedView;
ViewState ResolveView()
{
	return GetPrimaryView();
}


#endif