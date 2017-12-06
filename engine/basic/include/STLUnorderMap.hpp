#ifndef _STL_UnorderdMap_H_
#define _STL_UnorderdMap_H_
#pragma once
#include <unordered_map>
#include "serialization/Archive.hpp"
namespace Air
{
	Archive& operator << (Archive& ar, std::unordered_map<uint32_t, uint8_t> const & mp);

	Archive& operator << (Archive& ar, std::unordered_map<std::string, std::string> const & mp);

	Archive& operator << (Archive& ar, std::unordered_map<std::string, uint32_t> const & mp);

	Archive& operator << (Archive & ar, std::unordered_map<std::string, struct ResourceTableEntry> const & mp);

	Archive& operator << (Archive & ar, std::unordered_map<std::string, std::string> const & mp);

	Archive& operator << (Archive & ar, std::unordered_map<std::string, uint16_t> const & mp);
}
#endif
