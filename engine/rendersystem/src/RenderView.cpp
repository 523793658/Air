#include "Engine.h"
#include "rendersystem/include/RenderView.hpp"

namespace Air
{
	void RenderView::onBind(FrameBuffer& fb, uint32_t att)
	{

	}
	void RenderView::onUnbind(FrameBuffer& fb, uint32_t att)
	{

	}

	Texture* RenderView::getSrcTexture()
	{
		return mSrcTexture;
	}

	void UnorderAccessView::onBind(FrameBuffer& fb, uint32_t att)
	{

	}

	void UnorderAccessView::onUnbind(FrameBuffer& fb, uint32_t att)
	{

	}

}