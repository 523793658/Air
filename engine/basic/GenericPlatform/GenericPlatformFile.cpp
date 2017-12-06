#include "basic/include/Basic.h"
#include "basic/include/StringView.hpp"
#include "GenericPlatform/GenericPlatformFile.h"
#include "HAL/FileSystem.h"
bool IPlatformFile::fileExist(std::string const & filename)
{
	return filesystem::exists(filename);
}
