#ifndef _StaticMeshComponent_H_
#define _StaticMeshComponent_H_
#pragma once
#include "PreDeclare.h"
#include "ComponentBase.hpp"
namespace Air
{
	class AIR_CORE_API StaticMeshComponent : public ComponentBase
	{
	public:

	private:
		MeshPtr mMesh;
	};
}
#endif
