#ifndef _BASIC_TYPES_H_
#define _BASIC_TYPES_H_
#pragma once

#include <cstdint>
#include "CPP11.h"

#ifdef AIR_COMPILER_MSVC
#define AIR_RESTRICT __restrict
#define AIR_ASSUME(x) (__assume(x))
#else
#define AIR_RESTRICT
#define AIR_ASSUME(x) (BOOST_ASSERT(x))
#endif





#endif // !_BASIC_TYPES_H_
