#include "Context.h"
#include "rendersystem/include/RenderView.hpp"

namespace Air
{
	void RenderView::onBind(FrameBuffer& fb, uint32_t att)
	{

	}
	void RenderView::onUnbind(FrameBuffer& fb, uint32_t att)
	{

	}

	TexturePtr const & RenderView::getSrcTexture() const
	{
		if (mSrcTexture.get() == nullptr)
		{
			BOOST_ASSERT(false);
		}
		return mSrcTexture;
	}

	void UnorderAccessView::onBind(FrameBuffer& fb, uint32_t att)
	{

	}

	void UnorderAccessView::onUnbind(FrameBuffer& fb, uint32_t att)
	{

	}

}