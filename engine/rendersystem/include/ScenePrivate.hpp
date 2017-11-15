#ifndef _ScenePrivate_H_
#define _ScenePrivate_H_
#pragma once
#include "rendersystem/include/SceneInterface.hpp"
namespace Air
{
	class World;

	class Scene : public SceneInterface
	{
	public:

		struct ReadOnlyCVARCache
		{
			ReadOnlyCVARCache();
			bool bEnblePointLightShadow;
			bool bEnableStaionarySkyLight;
			bool bEnableAtomsphericFog;
			bool bEnableLowQualityLightmaps;
			bool bEnableVertexFoggingforOpaque;
		};

		World* mWrold;
		class FFXSystemInterface* mFXSystem;






	};

}



#endif
