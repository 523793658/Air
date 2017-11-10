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
		PPT_HDR,

		PPT_CUSTOM			//自定义后处理类型
	};

	struct PostProcessConfig 
	{
		uint32_t mType;
		std::string mName;
		PostProcessChain * mChain;
		virtual ~PostProcessConfig() = 0
		{
		}
	};

	class AIR_CORE_API PostProcesser
	{
	public:
		PostProcesser();
		PostProcesser(PostProcessChain* chain);
		~PostProcesser();

		virtual void update() = 0;
		virtual void render() = 0;

		virtual void setOutputFrameBuffer(FrameBufferPtr const & output);
		virtual void setInputTexture(uint16_t index, TexturePtr srcTex);

		virtual void setOuputTexture(uint16_t index, TexturePtr const & tex);

		virtual void setConfig(PostProcessConfigPtr config);

		virtual void onRenderBegin();

		virtual void onRenderEnd();

	protected:

		PostProcessConfigPtr mConfig;
		std::vector<TexturePtr> mSrcTextures;
		FrameBufferPtr mOutputFrameBuffer;
		std::vector<TexturePtr> mOutputTexture;
		bool mDirty{ true };
	};


	class AIR_CORE_API PostProcesserCreator
	{
	public:
		virtual std::shared_ptr<PostProcessConfig> loadCfg(XMLNodePtr const & node) = 0;
		virtual PostProcesser* createInstance(std::shared_ptr<PostProcessConfig> const & cfg) = 0;
	};

	class AIR_CORE_API PostProcessChain
	{
	public:
		PostProcessChain();

		void assemblePostProcessChain();

		void update();

		void loadCfg(std::string cfgPath);

		void loadCfg(XMLNodePtr node);

		RenderablePtr const & getRenderable() const;

		void setInputTarget(FrameBufferPtr const & fb);

		void setOutputTarget(FrameBufferPtr const & fb);


	public:
		static void registerProcesser(std::string name, PostProcesserCreator* creater);
		static PostProcesserCreator* getProcesserCreator(std::string name);
	private:
		static std::unordered_map<std::string, PostProcesserCreator*> mProcesserCreators;
	private:
		PostProcesser* createPostProcesser(PostProcessConfigPtr config);

	private:
		std::vector<PostProcessConfigPtr> mConfigs;

		ElementFormat mColorFormat;

		RenderablePtr mRenderable;

		float mIsDirty{ true };

		FrameBufferPtr mInputframeBuffer;
		FrameBufferPtr mOutputFrameBuffer;

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
