#ifndef _Air_Frame_Buffer_H_
#define _Air_Frame_Buffer_H_
#pragma once

#include "PreDeclare.h"
#include <vector>

namespace Air
{
	class UnorderAccessView;

	class AIR_CORE_API FrameBuffer
	{
	public:
		enum Attachment
		{
			ATT_DepthStencil,
			ATT_Color0,
			ATT_Color1,
			ATT_Color2,
			ATT_Color3,
			ATT_Color4,
			ATT_Color5,
			ATT_Color6,
			ATT_Color7
		};

		enum ClearBufferMask
		{
			CBM_Color = 1UL << 0,
			CBM_Depth = 1UL << 1,
			CBM_Stencil = 1UL << 2
		};

	public:
		FrameBuffer();
		virtual ~FrameBuffer();
		virtual std::wstring const & getDescription() const = 0;

		uint32_t getLeft() const;
		uint32_t getTop() const;
		uint32_t getWidth() const;
		uint32_t getHeight() const;

		ViewportPtr const & getViewport() const;
		ViewportPtr& getViewport();

		void setViewport(ViewportPtr const & viewport);

		void attach(uint32_t att, RenderViewPtr renderview);
		void detach(uint32_t att);
		RenderViewPtr getAttached(uint32_t att) const;

		virtual void onBind();
		virtual void onUnbind();

		virtual void discard(uint32_t flags) = 0;

		virtual void swapBuffers()
		{

		}

		bool isDirty() const
		{
			return mViewsDirty;
		}

		virtual void clear(uint32_t flags, Color const& clr, float depth, int32_t stencil) = 0;

		void setClearFlag(bool needClear, uint32_t clearFlag);

	protected:
		bool mViewsDirty;
		bool mNeedClear{ false };
		uint32_t mClearFlag;
		uint32_t mLeft;
		uint32_t mTop;
		uint32_t mWidth;
		uint32_t mHeight;
		ViewportPtr mViewport;
		std::vector<RenderViewPtr> mColorViews;
		RenderViewPtr mDepthStencilView;

		std::vector<std::shared_ptr<UnorderAccessView>> mUAViews;

	public:
#ifdef AIR_DEBUG
		void setDebugInfo(std::string info)
		{
			mDebugInfo = info;
		}
#else
		void inline setDebugInfo(std::string_view info)
		{
			AIR_UNREACHABLE(info);
		}
#endif

#ifdef AIR_DEBUG
	private:
		std::string mDebugInfo;
#endif

	};


}



#endif