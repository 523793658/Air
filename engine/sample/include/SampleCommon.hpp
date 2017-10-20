#ifndef _Sample_Common_H_
#define _Sample_Common_H_
#pragma once
#include "PreDeclare.h"
#include "scene_manager/include/SceneObjectHelper.hpp"

#ifndef SAMPLE_COMMON_SOURCE
#endif

int SampleMain();

inline int entryFunc()
{
	return SampleMain();
}

namespace Sample
{
	using namespace Air;
	class SphereObject : public SceneObjectHelper
	{
	public:
		SphereObject(float3 const & diff, float3 const &spec, float glossiness, float metallic);

		void setRenderingType(int type)
		{

		}

	};

	class CubeObject : public SceneObjectHelper
	{
	public:
		CubeObject(float4 const & diff, float4 const & spec, float glossiness);
	};
}



#endif // ifndef _Sample_Common_H_

