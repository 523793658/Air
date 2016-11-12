#include "basic/include/Basic.h"
#include <boost/lexical_cast.hpp>
#include "basic/include/ThrowErr.hpp"
namespace Air
{
	std::string combineFileLine(std::string const & file, int line)
	{
		return file + ": " + boost::lexical_cast<std::string>(line);
	}
}