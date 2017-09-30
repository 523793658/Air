#ifndef _B_ERRORHANDING_H_
#define _B_ERRORHANDING_H_
#pragma once
#include <string>
#include <stdexcept>
#include "basic/include/cpp17.h"
#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef long HRESULT;
#endif

namespace Air
{
	std::string combineFileLine(std::string_view, int line);
	void Verify(bool x);

#if defined(AIR_DEBUG) || !defined(AIR_BUILTIN_UNREACHABLE)
	AIR_ATTRIBUTE_NORETURN void AIRUnreachableInternal(char const * msg = nullptr, char const * file = nullptr, uint32_t line = 0);
#endif
}

#define TEC(x)		{throw std::system_error(x, Air::combineFileLine(__FILE__, __LINE__));}

#define TMSG(msg)	{throw std::runtime_error(msg);}

#define  TIFEC(x)	{if(x) TEC(x)}

#define		TERRC(x) TEC(std::make_error_code(x))

#define TIFERRC(x)		TIFEC(std::make_error_code(x))

#define TIFHR(x)		{if(static_cast<HRESULT>(x) < 0) { TMSG(Air::combineFileLine(__FILE__, __LINE__));}}

#ifdef AIR_DEBUG
#define AIR_UNREACHABLE(msg) Air::AIRUnreachableInternal(msg, __FILE__, __LINE__)

#elif defined(AIR_BUILTIN_UNREACHABLE)
#define AIR_UNREACHABLE(msg) AIR_BUILTIN_UNREACHABLE
#else
#define AIR_UNREACHABLE(msg) Air::AIRUnreachableInternal()
#endif // AIR_DEBUG



#endif
