#ifndef _Basic_Throw_err_H_
#define _Basic_Throw_err_H_

#pragma once
#include <string>
#include <stdexcept>
#include <system_error>

namespace Air
{
#if defined(AIR_COMPILER_MSVC) && (AIR_COMPILER_VERSION < 120)
	namespace errc = std::errc;
#else
	using std::errc;
#endif
}
#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef long HRESULT;
#endif

namespace Air
{
	std::string combineFileLine(std::string const & file, int line);
}

#define THR(x)		{throw std::system_error(std::make_error_code(x), Air::combineFileLine(__FILE__, __LINE__));}

#define TIF(x)		{HRESULT _hr = x; if(static_cast<HRESULT>(_hr) < 0){throw std::runtime_error(Air::combineFileLine(__FILE__, __LINE__));}}

namespace Air
{
	inline void Verify(bool x)
	{
		if (!x)
		{
			THR(errc::function_not_supported);
		}
	}
}



#endif