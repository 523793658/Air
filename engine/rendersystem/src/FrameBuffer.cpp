#include "Engine.h"
#include "basic/include/ThrowErr.hpp"
#include "rendersystem/include/RenderView.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderEngine.hpp"
#include "rendersystem/include/Viewport.hpp"

#include "rendersystem/include/FrameBuffer.hpp"

namespace Air
{
	FrameBuffer::FrameBuffer()
		:mLeft(0), mTop(0), mWidth(0), mHeight(0), mViewport(MakeSharedPtr<Viewport>())
	{

	}

	FrameBuffer::~FrameBuffer()
	{

	}

	uint32_t FrameBuffer::getLeft() const
	{
		return mLeft;
	}
	uint32_t FrameBuffer::getTop() const
	{
		return mTop;
	}
	uint32_t FrameBuffer::getWidth() const
	{
		return mWidth;
	}
	uint32_t FrameBuffer::getHeight() const
	{
		return mHeight;
	}

	ViewportPtr const & FrameBuffer::getViewport() const
	{
		return mViewport;
	}
	ViewportPtr& FrameBuffer::getViewport()
	{
		return mViewport;
	}

	void FrameBuffer::setViewport(ViewportPtr const & viewport)
	{
		mViewport = viewport;
	}

	void FrameBuffer::attach(uint32_t att, RenderViewPtr renderview)
	{
		switch (att)
		{
		case ATT_DepthStencil:
		{
			if (mDepthStencilView)
			{
				this->detach(att);
			}
			mDepthStencilView = renderview;
		}
		break;
		default:
		{
			BOOST_ASSERT(att >= ATT_Color0);
			RenderEngine& re = Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
			if (att >= static_cast<uint32_t>(ATT_Color0 + re.getDeviceCaps().mMaxSimultaneousRts))
			{
				THR(errc::function_not_supported);
			}
			uint32_t colorId = att - ATT_Color0;
			if ((colorId < mColorViews.size()) && mColorViews[colorId])
			{
				this->detach(att);
			}
			if (mColorViews.size() < colorId + 1)
			{
				mColorViews.resize(colorId + 1);
			}
			mColorViews[colorId] = renderview;
			size_t minColorIndex = colorId;
			for (size_t i = 0; i < colorId; ++i)
			{
				if (mColorViews[i])
				{
					minColorIndex = i;
				}
			}
			if (minColorIndex == colorId)
			{
				mWidth = renderview->getWidth();
				mHeight = renderview->getHeight();

				mViewport->mLeft = 0;
				mViewport->mTop = 0;
				mViewport->mWidth = mWidth;
				mViewport->mHeight = mHeight;
			}
		}
		break;
		}
		if (renderview)
		{
			renderview->onAttached(*this, att);
		}
		mViewsDirty = true;
	}
	void FrameBuffer::detach(uint32_t att)
	{
		switch (att)
		{
		case ATT_DepthStencil:
		{
			mDepthStencilView.reset();
		}
		break;
		default:
		{
			RenderEngine &re = Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
			if (att >= static_cast<uint32_t>(ATT_Color0 + re.getDeviceCaps().mMaxSimultaneousRts))
			{
				THR(errc::function_not_supported);
			}
			uint32_t colorId = att - ATT_Color0;
			if ((mColorViews.size() >= colorId + 1) && mColorViews[colorId])
			{
				mColorViews[colorId]->onDetached(*this, att);
				mColorViews[colorId].reset();
			}
		}
			break;
		}
		mViewsDirty = true;
	}
	RenderViewPtr FrameBuffer::getAttached(uint32_t att) const
	{
		switch (att)
		{
		case ATT_DepthStencil:
			return mDepthStencilView;
		default:
		{
			uint32_t colorId = att - ATT_Color0;
			if (mColorViews.size() < colorId + 1)
			{
				return RenderViewPtr();
			}
			else
			{
				return mColorViews[colorId];
			}
		}
			break;
		}
	}

	void FrameBuffer::onBind()
	{
		for (uint32_t i = 0; i < mColorViews.size(); ++i)
		{
			if (mColorViews[i])
			{
				mColorViews[i]->onBind(*this, ATT_Color0 + i);
			}
		}
		if (mDepthStencilView)
		{
			mDepthStencilView->onBind(*this, ATT_DepthStencil);
		}
		//for (uint32_t i = 0; i < )
		mViewsDirty = false;
	}

	void FrameBuffer::onUnbind()
	{
		for (uint32_t i = 0; i < mColorViews.size(); ++i)
		{
			if (mColorViews[i])
			{
				mColorViews[i]->onUnbind(*this, ATT_Color0 + i);
			}
		}
		if (mDepthStencilView)
		{
			mDepthStencilView->onUnbind(*this, ATT_DepthStencil);
		}

	}
}