#ifndef _STLVector_H_
#define _STLVector_H_
#pragma once
#include <vector>
#include "serialization/Archive.hpp"
namespace Air
{


	Archive& operator << (Archive & ar, std::vector<uint32_t> const & a);

	Archive& operator << (Archive & ar, std::vector<std::string> const & a);
}
#endif
