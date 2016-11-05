#ifndef _CoreCfg_H_
#define _CoreCfg_H_
#pragma once

#include <basic\include\Basic.h>


#define AIR_NAME		AIR


#ifdef AIR_CORE_SOURCE
#	define AIR_CORE_API AIR_SYMBOL_EXPORT
#else
#	define AIR_CORE_API	AIR_SYMBOL_IMPORT
#endif

#include <vector>
#include <string>

#include "PreDeclare.h"
#include "Context.h"



#endif