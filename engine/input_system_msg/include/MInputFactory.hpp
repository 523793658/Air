#ifndef _MInputFactory_H_
#define _MInputFactory_H_

#pragma  once

#include "PreDeclare.h"

#ifdef AIR_INPUT_MSG_IE_SOURCE
#define AIR_MINPUT_IE_API AIR_SYMBOL_EXPORT
#else
#define AIR_MINPUT_IE_API AIR_SYMBOL_IMPORT
#endif

extern "C"
{
	AIR_MINPUT_IE_API void makeInputFactory(std::unique_ptr<Air::InputFactory>& ptr);
}


#endif
