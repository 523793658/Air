#ifndef _SceneRenderTarget_H_
#define _SceneRenderTarget_H_
#pragma once
#include "rendersystem/include/RenderResource.hpp"
#include "rendersystem/include/CommandList.hpp"
namespace Air
{
	class AIR_CORE_API SceneRenderTargets : public RenderResource
	{
	public:
		virtual ~SceneRenderTargets() {}
		static SceneRenderTargets& get(CommandList& cmdList);
		static SceneRenderTargets& get(CommandListImmediate& cmdList);
	};
}
#endif
