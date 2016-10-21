#ifndef _CPP11_H_
#define _CPP11_H_
#pragma once


#include "CPP11.h"
#ifdef AIR_CXX11_CORE_NOEXCEPT_SUPPORT
#	define AIR_NOEXCEPT noexcept
#	define AIR_NOEXCPET_IF(predicate) noexcept((predicate))
#	define AIR_NOEXCPET_EXPR(expression) noexcept((expression))
#else
#	define	AIR_NOEXCEPT throw()
#	define	AIR_NOEXCPET_IF(predicate)
#	define	KLAYGE_NOEXCEPT_EXPR(expression) false
#endif

#ifdef AIR_CPP11_CORE_CONSTEXPR_SUPPORT
#	define AIR_CONSTEXPR	constexpr
#else
#	define AIR_CONSTEXPR
#endif
#endif