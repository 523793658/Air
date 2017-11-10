#ifndef _ShadowMap_H_
#define _ShadowMap_H_
#pragma once
namespace Air
{
	class AIR_CORE_API ShadowUtil
	{
	public:
		static void adjustShadowCamera(Camera* srcCamera, Camera* dstCamera, float3 const & dir, float factor, float factor2);

	};
}









#endif
