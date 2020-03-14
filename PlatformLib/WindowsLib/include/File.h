#pragma once

#include "../../interface/IFile.h"
#include <stdio.h>


class File : public IFile
{
public:
	File();
	virtual ~File();

	virtual bool Open(const char* path, FileMode mode);
	virtual void Close() = 0;
	virtual void GetData(char* data, uint32 length = 0);
	virtual uint32 GetFileSize();
private:
	FileMode mode;
	FILE* file;
};
