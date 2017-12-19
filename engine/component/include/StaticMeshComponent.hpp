#ifndef _StaticMeshComponent_H_
#define _StaticMeshComponent_H_
#pragma once
#include "PreDeclare.h"
#include "PrimitiveComponent.hpp"
namespace Air
{
	class AIR_CORE_API StaticMeshComponent : public PrimitiveComponent
	{
	public:
		virtual void onRegister() override;

	private:
		StaticMeshPtr mStaticMesh;
	};
}
#endif
