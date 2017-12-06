#ifndef _Engine_H_
#define _Engine_H_
#pragma once

#include <basic\include\Basic.h>


#define AIR_NAME		Air


#ifdef AIR_CORE_SOURCE
#	define AIR_CORE_API AIR_SYMBOL_EXPORT
#else
#	define AIR_CORE_API	AIR_SYMBOL_IMPORT
#endif
#include <vector>
#include <string>
#include <unordered_map>
#include "basic/include/ErrorHanding.hpp"
#include "boost/assert.hpp"
#include "PreDeclare.h"
#include "basic/include/Util.h"
#include "basic/include/COMPtr.hpp"

#endif