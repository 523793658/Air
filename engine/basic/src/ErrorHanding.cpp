#include "basic/include/Basic.h"
#include <system_error>
#include "basic/include/Log.hpp"
#if defined(AIR_COMPILER_CLANGC2)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif

#include <boost/lexical_cast.hpp>
#if defined(AIR_COMPILER_CLANGC2)
#pragma clang diagnostic pop
#endif



#include "basic/include/ErrorHanding.hpp"

namespace Air
{
	std::string combineFileLine(std::string_view file, int line)
	{
		return std::string(file) + ": " + boost::lexical_cast<std::string>(line);
	}
	void Verify(bool x)
	{
		if (!x)
		{
			TERRC(std::errc::function_not_supported);
		}
	}

#if !defined(NDEBUG) || !defined(LLVM_BUILTIN_UNREACHABLE)



	AIR_ATTRIBUTE_NORETURN void AIRUnreachableInternal(char const * msg, char const * file, uint32_t line)
	{
		if (msg)
		{
			logError(msg);
		}
		if (file)
		{
			logError("UNREACHABLE executed at %s: %d.", file, line);
		}
		else
		{
			logError("UNREACHABLE executed.");
		}
		TMSG("Unreachable.");
	}
#endif
}