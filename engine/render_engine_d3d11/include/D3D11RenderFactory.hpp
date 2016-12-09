#ifndef _D3D11RenderFactory_H_
#define _D3D11RenderFactory_H_

#pragma once

#include <core/include/PreDeclare.h>
#include "rendersystem/include/RenderFactory.h"

#ifdef AIR_D3D11_RE_SOURCE
#define AIR_D3D11_RE_API AIR_SYMBOL_EXPORT
#else
#define AIR_D3D11_RE_API AIR_SYMBOL_IMPORT
#endif

extern "C"
{
	AIR_D3D11_RE_API void makeRenderFactory(std::unique_ptr<Air::RenderFactory>& ptr);
}



#endif