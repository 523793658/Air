#ifndef _D3D11RenderFactory_Internal_H_
#define _D3D11RenderFactory_Internal_H_
#pragma once

#include "core/include/PreDeclare.h"
#include "rendersystem/include/RenderFactory.h"

namespace Air
{
	class D3D11RenderFactory : public RenderFactory
	{
	public:
		D3D11RenderFactory();
		std::wstring const & getName() const;

		RenderLayoutPtr MakeRenderLayout();
	private:
		virtual std::unique_ptr<RenderEngine> doMakeRenderEngine() override;
	};
}


#endif