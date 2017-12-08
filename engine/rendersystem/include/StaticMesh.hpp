#ifndef _StaticMesh_H_
#define _StaticMesh_H_
#pragma once
#include "Context.h"
namespace Air
{
	class AStaticMesh
	{
		std::unique_ptr<class StaticMeshRenderData> mRenderData;
	};
}
#endif
