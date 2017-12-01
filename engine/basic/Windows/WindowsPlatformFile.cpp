#include "basic/include/Basic.h"
#include "boost/assert.hpp"
#include "HAL/FileSystem.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Windows/WindowsHWrapper.h"

namespace FileConstants
{
	uint32_t WIN_INVALID_SET_FILE_POINTER = INVALID_SET_FILE_POINTER;
}

class FileHandleWindows : public IFileHandle
{
	enum { READWRITE_SZIE = 1024 * 1024 };

	HANDLE mFileHandle;

	FORCEINLINE int64_t fileSeek(int64_t distance, uint32_t moveMethod)
	{
		LARGE_INTEGER li;
		li.QuadPart = distance;
		li.LowPart = SetFilePointer(mFileHandle, li.LowPart, &li.HighPart, moveMethod);
		if (li.LowPart == FileConstants::WIN_INVALID_SET_FILE_POINTER&& GetLastError != NO_ERROR)
		{
			li.QuadPart = -1;
		}
		return li.QuadPart;
	}

public:
	FileHandleWindows(HANDLE fileHandle = NULL)
		:mFileHandle(fileHandle)
	{}
	
	virtual bool seek(int64_t newPosition) override
	{
		BOOST_ASSERT(isVaild());
		BOOST_ASSERT(newPosition >= 0);
		return fileSeek(newPosition, FILE_BEGIN);
	}

	virtual bool seekFromEnd(int64_t NewPostionRelativeToEnd = 0) override
	{
		BOOST_ASSERT(isVaild());
		BOOST_ASSERT(NewPostionRelativeToEnd <= 0);
		return fileSeek(NewPostionRelativeToEnd, FILE_END) != -1;
	}

	virtual int64_t tell()
	{
		BOOST_ASSERT(isVaild());
		return fileSeek(0, FILE_CURRENT);
	}

	FORCEINLINE bool isVaild()
	{
		return mFileHandle != NULL && mFileHandle != INVALID_HANDLE_VALUE;
	}
};


class WindowsPlatformFile : public IPhysicalPlatformFile
{
public:
	virtual IFileHandle* openWrite(const TCHAR * fileName, bool bAppend = false, bool bAllowRead = false)
	{
		uint32_t Access = GENERIC_WRITE;
		uint32_t WinFlags = bAllowRead ? FILE_SHARE_READ : 0;
		uint32_t Create = bAppend ? OPEN_ALWAYS : CREATE_ALWAYS;
		HANDLE Handle = CreateFileW(filesystem::absolute(fileName).c_str(), Access, WinFlags, NULL, Create, FILE_ATTRIBUTE_NORMAL, NULL);
		if (Handle != INVALID_HANDLE_VALUE)
		{
			FileHandleWindows* platformFileHandle = new FileHandleWindows(Handle);
			if (bAppend)
			{
				platformFileHandle->seekFromEnd(0);
			}
			return platformFileHandle;
		}
		return nullptr;
	}

	virtual bool setReadOnly(const wchar_t* filename, bool bNeedReadOnlyValue) override
	{
		return !!SetFileAttributesW(filesystem::absolute(filename).c_str(), bNeedReadOnlyValue ? FILE_ATTRIBUTE_READONLY : FILE_ATTRIBUTE_NORMAL);
	}

};