#ifndef _GenericPlatformFile_H_
#define _GenericPlatformFile_H_
#pragma once
#include <string>
class IFileHandle
{
public:
	virtual ~IFileHandle()
	{}
	virtual int64_t tell() = 0;

	virtual bool seekFromEnd(int64_t NewPostionRelativeToEnd = 0) = 0;
	virtual bool seek(int64_t newPosition = 0) = 0;



};


class IPlatformFile
{
public:
	virtual IFileHandle* openWrite(const wchar_t* fileName, bool bAppend = false, bool bAllowRead = false) = 0;

	virtual bool setReadOnly(const wchar_t* filename, bool bNeedReadOnlyValue) = 0;
};


class IPhysicalPlatformFile : public IPlatformFile
{

};

#endif
