#ifndef _Air_Render_View_H_
#define _Air_Render_View_H_
#pragma once;

#include "PreDeclare.h"
#include "ElementFormat.h"
#include "basic/include/Math.hpp"

namespace Air
{
	class SceneViewStateInterface;

	class AIR_CORE_API RenderView
	{



	public:

		virtual ~RenderView()
		{

		}
		uint32_t getWidth() const
		{
			return mWidth;
		}
		uint32_t getHeight() const
		{
			return mHeight;
		}

		ElementFormat getFormat() const
		{
			return mFormat;
		}

		virtual void clearColor(Color const & color) = 0;
		virtual void clearDepth(float depth) = 0;
		virtual void clearStencil(int32_t stencil) = 0;
		virtual void clearDepthStencil(float depth, int32_t stanecil) = 0;

		virtual void discard() = 0;

		virtual void onAttached(FrameBuffer& fb, uint32_t att) = 0;
		virtual void onDetached(FrameBuffer& fb, uint32_t att) = 0;

		virtual void onBind(FrameBuffer& fb, uint32_t att);
		virtual void onUnbind(FrameBuffer& fb, uint32_t att);

		TexturePtr const & getSrcTexture() const;

	protected:
		uint32_t mWidth;
		uint32_t mHeight;
		ElementFormat mFormat;
		TexturePtr mSrcTexture;
	};

	class AIR_CORE_API UnorderAccessView
	{
	public:
		UnorderAccessView()
			: mInitCount(0)
		{

		}
		virtual ~UnorderAccessView()
		{

		}

		uint32_t getWidth() const 
		{
			return mHeight;
		}

		uint32_t Height() const
		{
			return mHeight;
		}
		ElementFormat getFormat() const
		{
			return mFormat;
		}
		virtual void clear(float4 const & val) = 0;
		virtual void clear(uint4 const & val) = 0;
		virtual void discard() = 0;
		virtual void onAttached(FrameBuffer& fb, uint32_t att) = 0;
		virtual void onDetached(FrameBuffer& fb, uint32_t att) = 0;

		virtual void onBind(FrameBuffer& fb, uint32_t att);
		virtual void onUnbind(FrameBuffer& fb, uint32_t att);

		void setInitCount(uint32_t count)
		{
			mInitCount = count;
		}

		uint32_t getInitCount() const
		{
			return mInitCount;
		}

	private:
		uint32_t mWidth;
		uint32_t mHeight;
		ElementFormat mFormat;
		uint32_t mInitCount;
	};




}


#endif