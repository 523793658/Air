#ifndef _Air_Viewport_H_
#define _Air_Viewport_H_
#pragma once


#include "PreDeclare.h"
namespace Air
{
	struct AIR_CORE_API Viewport
	{
		Viewport();
		Viewport(int left, int top, int width, int height);
		int mLeft;
		int mTop;
		int mWidth;
		int mHeight;
		CameraPtr  mCamera;
	};
}



#endif