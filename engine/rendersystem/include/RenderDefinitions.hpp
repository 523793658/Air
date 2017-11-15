#ifndef _RenderDefinitions_H_
#define _RenderDefinitions_H_
#pragma once
namespace Air
{
	enum EUniformBufferUsage
	{
		UBU_SingleDraw = 0,
		UBU_SingleFrame,
		UBU_MultiFrame
	};

	enum EUniformBufferBaseType
	{
		UBMT_INVALID,
		UBMT_BOOL,
		UBMT_INT32,
		UBMT_UINT32,
		UBMT_FLOAT32,
		UBMT_STRUCT,
		UBMT_SRV,
		UBMT_UAV,
		UBMT_SAMPLER,
		UBMT_TEXTURE
	};

	inline bool isUniformBufferResourceType(EUniformBufferBaseType baseType)
	{
		return baseType == UBMT_SRV || baseType == UBMT_UAV || baseType == UBMT_STRUCT || baseType == UBMT_TEXTURE;
	}
}
#endif
