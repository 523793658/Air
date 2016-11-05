#ifndef _Air_Frame_Buffer_H_
#define _Air_Frame_Buffer_H_
#pragma once

#include "PreDeclare.h"
#include <vector>

namespace Air
{
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

		void attach(uint32_t att, RenderView)

	};


}



#endif