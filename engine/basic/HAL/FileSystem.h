#ifndef _FileSystem_H_
#define _FileSystem_H_
#pragma once

#include "HAL/CoreTypes.h"
#include "basic/serialization/Archive.hpp"
#if defined(AIR_TS_LIBRARY_FILESYSTEM_V3_SUPPORT)
#include <experimental/filesystem>
#elif defined(AIR_TS_LIBRARY_FILESYSTEM_V2_SUPPORT)
#include <filesystem>
namespace std
{
	namespace experimental
	{
		namespace filesystem = std::tr2::sys;
	}
}
#endif

using namespace std;
using namespace experimental;

enum EFileWrite
{
	FILEWRITE_None = 0x00,
	FILEWRITE_NoFail = 0x01,
	FILEWRITE_NoReplaceExisting = 0x02,
	FILEWRITE_EvenIfReadOnly = 0x04,
	FILEWRITE_Append = 0x08,
	FILEWRITE_AllowRead = 0x10
};

enum EFileRead
{
	FILEREAD_None = 0x00,
	FILEREAD_NoFail = 0x01,
	FILEREAD_Silent = 0x02,
	FILEREAD_AllowWrite = 0x04,
};


enum ECopyResult
{
	COPY_OK = 0x00,
	COPY_Fail = 0x01,
	COPY_Canceled = 0x02,
};


class IFileSystem
{
protected:
	IFileSystem(){}
public:
	static IFileSystem& get();

	virtual Air::Archive* createFileWriter(const wchar_t* fileName, uint32_t writeflags = 0) = 0;

	virtual Air::Archive* createFileReader(std::string const &, uint32_t flags) = 0 ;

	virtual bool createDirectory(const wchar_t* fileName, bool) = 0;

	virtual bool deleteFile(std::string const &, bool requireExists = false, bool evenReadOnly = false, bool quiet = false) = 0;

	virtual bool move(const char* dst, const char* src, bool replace = true, bool evenIfReadOnly = false, bool attributes = false, bool bDonotRetryOnError = false);

	virtual std::string convertToAbsolutePathForExternalAppForWrite(std::string const & absolutePath);
};




#endif