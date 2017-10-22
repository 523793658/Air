#ifndef _PostProcess_H_
#define _PostProcess_H_
#pragma once
#include "PreDeclare.h"
#include <unordered_map>
#include <stack>
namespace Air
{

	enum PostProcessType
	{
		PPT_ToneMapping
	};

	struct PostProcessConfig 
	{
		PostProcessType mType;
	};

	class AIR_CORE_API PostProcesser
	{
	public:
		PostProcesser();
		PostProcesser(PostProcessChain* chain);
		~PostProcesser();

		virtual void update();
		virtual void render();

		virtual void setOutputFrameBuffer(FrameBufferPtr const & output);
		virtual void setInputTexture(uint16_t index, TexturePtr srcTex);

	protected:
		std::vector<TexturePtr> mSrcTextures;
		FrameBufferPtr mOutputFrameBuffer;
		PostProcessChain * mChain;
	};


	class AIR_CORE_API PostProcessChain
	{
	public:
		PostProcessChain();

		void assemblePostProcessChain();

		void update();

		void loadCfg(std::string cfgPath);

		RenderablePtr const & getRenderable() const;

	private:
		PostProcesser* createPostProcesser(PostProcessConfig config);

	private:
		std::vector<PostProcessConfig> mConfigs;

		ElementFormat mColorFormat;

		RenderablePtr mRenderable;

		float mIsDirty{ true };

		FrameBufferPtr mSceneFrameBuffer;
		std::stack<FrameBufferPtr> mTempFrameBuffers;
		std::unordered_map<std::string, FrameBufferPtr> mFrameBufferMap;
	};

	class AIR_CORE_API PostProcesserRegister
	{
	private:
		std::unordered_map<uint32_t, 
	};
}




#endif
