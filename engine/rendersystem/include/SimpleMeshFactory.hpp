#ifndef _SimpleMeshFactory_H_
#define _SimpleMeshFactory_H_
#pragma  once
#include "PreDeclare.h"

namespace Air
{
	class AIR_CORE_API SimpleMeshFactory {
	public:
		struct VertexData 
		{
			float3 pos;
			float3 normal;
			float2 texCoord;
		};

	public:
		static StaticMeshPtr createStaticShpere(float radius);
	};
}




#endif