#ifndef _PostProcess_H_
#define _PostProcess_H_
#pragma once
#include "PreDeclare.h"
#include <unordered_map>
namespace Air
{
	enum PostProcessType
	{

	};

	struct PostProcessConfig 
	{
		PostProcessType mType;
		std::string mInputFrameBuffer;
	};

	class AIR_CORE_API PostProcess
	{
	public:
		PostProcess();
		~PostProcess();
	};


	class AIR_CORE_API PostProcessChain
	{
	public:
		PostProcessChain();

		void assemblePostProcessChain();

		void update();

		void loadCfg(std::string cfgPath);
	private:

		RenderablePtr mRenderable;

		float mIsDirty{ true };

		FrameBufferPtr mSceneFrameBuffer;

		std::vector<FrameBufferPtr> mFrameBuffers;

		std::unordered_map<std::string, FrameBufferPtr> mFrameBufferMap;
	};
}




#endif
