#ifndef _GenericPlatformFileSystem_H_
#define _GenericPlatformFileSystem_H_
#pragma once
#include "HAL/CoreTypes.h"

#include "HAL/FileSystem.h"
#include "HAL/PlatformFileSystem.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "serialization/Archive.hpp"
class ArchiveFileWriterGeneric : public Air::Archive
{
public:
	ArchiveFileWriterGeneric(IFileHandle* handle, const wchar_t* filename, int64_t pos);
	~ArchiveFileWriterGeneric();


};


class FileSystemGeneric
	: public IFileSystem
{
	FORCEINLINE IPlatformFile& getLowLevel() const
	{
		return PlatformFileSystem::get().getPlatformFile();
	}

public:
	Air::Archive* createFileWriter(const wchar_t* fileName, uint32_t writeflags = 0) override;

	bool createDirectory(const wchar_t* fileName, bool) override;
};



#endif
