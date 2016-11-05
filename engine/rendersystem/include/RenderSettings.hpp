#ifndef _RenderSettings_H_
#define _RenderSettings_H_
#include <vector>
#include "ElementFormat.h"
namespace Air
{
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
		std::vector<std::pair<std::string, std::string>> options;
	};
}
#endif