#ifndef _RenderableHelper_H_
#define _RenderableHelper_H_
#pragma once

#include "rendersystem/include/Renderable.hpp"

namespace Air
{
	class AIR_CORE_API RenderableHelper : public Renderable
	{
	public:
		explicit RenderableHelper(std::wstring const & name);
		virtual ~RenderableHelper();
	};
}

#endif