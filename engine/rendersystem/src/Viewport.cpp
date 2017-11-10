#include "Context.h"
#include "basic/include/Util.h"
#include "Camera.hpp"

#include "rendersystem/include/Viewport.hpp"

namespace Air
{
	Viewport::Viewport()
		:mCamera(MakeSharedPtr<Camera>())
	{

	}

	Viewport::Viewport(int left, int top, int width, int height)
		: mLeft(left), mTop(top), mWidth(width), mHeight(height), mCamera(MakeSharedPtr<Camera>())
	{

	}
}