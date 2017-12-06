#ifndef _StringView_H_
#define _StringView_H_
#pragma once
#include "basic/include/Config.h"
#if defined(AIR_CXX17_LIBRARY_STRING_VIEW_SUPPORT)
#include <string_view>
#else
#include <boost/utility/string_view.hpp>
namespace std
{
	using boost::basic_string_view;
	using boost::string_view;
	using boost::wstring_view;
}

#endif


#endif
