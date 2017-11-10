#ifndef _RenderSettings_H_
#define _RenderSettings_H_
#include <vector>
#include "ElementFormat.h"

namespace Air
{
	struct RenderPassConfig
	{
		uint32_t mType;
		std::string mOutputTargetName;
	};

	struct RenderLayerConfig
	{
		std::vector<RenderPassConfig> mPassesConfig;
	};

	struct PipelineConfigs
	{
		std::string mPostProcessPath;
		std::vector<RenderLayerConfig> mLayersConfig;
	};


	struct RenderSettings
	{
		
		bool hide_win;
		bool full_screen;
		int left;
		int top;
		int width;
		int height;
		ElementFormat mColorFmt;
		ElementFormat mDepthStencilFmt;
		uint32_t mSampleCount;
		uint32_t mSampleQuality;
		uint32_t mSyncInterval;
		uint32_t mMotionFrames;
		StereoMethod mStereoMethod;
		float mStereoSeparation;
		bool mHdr;
		bool mPostProcessEnable;

		std::string mPostProcessCfgPath;
		std::string mPipelineConfigPath;
		std::vector<std::pair<std::string, std::string>> options;
	};
}
#endif