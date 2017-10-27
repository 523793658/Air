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
		PPT_ToneMapping,

		PPT_CUSTOM			//自定义后处理类型
	};

	struct PostProcessConfig 
	{
		uint32_t mType;
		std::string mName;
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


	class AIR_CORE_API PostProcesserCreator
	{
	public:
		virtual std::shared_ptr<PostProcessConfig> loadCfg(XMLNodePtr const & node) = 0;
		virtual PostProcesser* createInstance(std::shared_ptr<PostProcessConfig> const & cfg, PostProcessChain* chain) = 0;
	};

	class AIR_CORE_API PostProcessChain
	{
	public:
		PostProcessChain();

		void assemblePostProcessChain();

		void update();

		void loadCfg(std::string cfgPath);

		RenderablePtr const & getRenderable() const;

	public:
		static void registerProcesser(std::string name, PostProcesserCreator* creater);

	private:
		static std::unordered_map<std::string, PostProcesserCreator*> mProcesserCreators;
	private:
		PostProcesser* createPostProcesser(PostProcessConfigPtr config);

	private:
		std::vector<PostProcessConfigPtr> mConfigs;

		ElementFormat mColorFormat;

		RenderablePtr mRenderable;

		float mIsDirty{ true };

		FrameBufferPtr mSceneFrameBuffer;
		std::stack<FrameBufferPtr> mTempFrameBuffers;

		std::vector<PostProcesser*> mPostProcessers;
	};
}

#define RegisterProcesser(name, creator) \
namespace {	\
class ObjectFactory##creator { \
public: \
	ObjectFactory##creator(){\
		Air::PostProcessChain::registerProcesser(name, new creator());	\
	}						\
private: \
	static ObjectFactory##creator instance;		\
}; \
	ObjectFactory##creator ObjectFactory##creator::instance;		\
}\




#endif
