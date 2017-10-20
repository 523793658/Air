#include "Engine.h"
#include "rendersystem/include/RenderableHelper.hpp"

namespace Air
{
	RenderableHelper::RenderableHelper(std::wstring const & name)
		:mName(name)
	{

	}
	RenderLayout& RenderableHelper::getRenderLayout() const
	{
		return *mRenderlayout;
	}

	AABBox const & RenderableHelper::getPosAABB() const
	{
		return mAABB;
	}

	AABBox const & RenderableHelper::getTexcoordAABB() const
	{
		return mTCAABB;
	}
	std::wstring const & RenderableHelper::getName() const
	{
		return mName;
	}

	RenderableHelper:: ~RenderableHelper()
	{

	}
}