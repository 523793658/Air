#ifndef _Basic_SIMD_Math_H_
#define _Basic_SIMD_Math_H_
#pragma once

#include "basic/include/PreDeclare.h"

#if defined(AIR_SSE_SUPPORT)
#	define SIMD_MATH_SSE
#else
#	define	SIMD_MATH_GENERAL
#endif




#endif