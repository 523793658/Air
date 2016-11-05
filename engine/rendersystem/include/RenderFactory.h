#ifndef _RenderFactory_H_
#define _RenderFactory_H_
#pragma once

#include "PreDeclare.h"
#include "RenderEngine.hpp"

namespace Air
{
	class AIR_CORE_API RenderFactory
	{
	public:
		virtual ~RenderFactory();

		virtual std::wstring const & getName() const = 0;

		RenderEngine& getRenderEngineInstance();

		virtual RenderLayoutPtr MakeRenderLayout() = 0;

	private:
		virtual std::unique_ptr<RenderEngine> doMakeRenderEngine() = 0;

	private:
		std::unique_ptr<RenderEngine> mRenderEnginePtr;
	};
}




#endif