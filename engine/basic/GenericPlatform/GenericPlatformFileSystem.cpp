#include "GenericPlatform/GenericPlatformFileSystem.hpp"
#include "basic/include/Basic.h"







bool FileSystemGeneric::createDirectory(const wchar_t* fileName, bool tree)
{
	if (tree)
	{
		return filesystem::create_directories(fileName);
	}
	else
	{
		return filesystem::create_directory(fileName);
	}
}

Air::Archive* FileSystemGeneric::createFileWriter(const wchar_t* filename, uint32_t writeflags = 0)
{
	filesystem::path path(filename);
	if (filesystem::exists(path))
	{

	}
	filesystem::create_directories(path.parent_path());
	if (writeflags & FILEWRITE_EvenIfReadOnly)
	{
		getLowLevel().setReadOnly(filename, false);
	}
	IFileHandle* handle = getLowLevel().openWrite(filename, !!(writeflags & FILEWRITE_Append), !!(writeflags & FILEWRITE_AllowRead));

	if (!handle)
	{

	}
	return new ArchiveFileWriterGeneric(handle, filename, handle->tell());
}

