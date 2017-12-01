#ifndef _RenderPipeline_H_
#define _RenderPipeline_H_
#pragma once

#include "PreDeclare.h"

namespace Air
{
	struct RenderPipelineConfig 
	{
		std::string mPostProcessConfigPath;
	};

	class AIR_CORE_API RenderPipeline
	{
	public:
		RenderPipeline();
		void setFrameBuffer(FrameBufferPtr const & fb);

		void setConfig();
		void resize();

		void loadConfig(std::string path);

		
		void rebuild();

		void update();

		void addGlobalResource(std::string const & name, std::shared_ptr<void>);

		std::shared_ptr<void> removeGloobalResource(std::string const & name);

		std::shared_ptr<void> getGlobalResource(std::string const & name);

		RenderEnvironment* getRenderEnvironment();
	private:
		void postProcess(bool skip);

		

	private:

		std::vector<RenderLayer*> mRenderLayers;

		FrameBufferPtr mScreenFrameBuffer;

		PostProcessChainPtr mPostProcessChain;

		RenderPipelineConfig mConfig;

		std::vector<FrameBufferPtr> mFrameBuffers;

		std::unordered_map<std::string, std::shared_ptr<void>> mGlobalResource;

		class RenderEnvironment* mRenderEnvironment;
	};




}




#endif
