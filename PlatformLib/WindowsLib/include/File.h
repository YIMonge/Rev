#pragma once

#include "../../interface/IFile.h"
#include <stdio.h>


class File : public IFile
{
public:
	File();
	virtual ~File();

	virtual void Open(const char* path, FileMode::Mode mode);
	virtual void Close();
	virtual uint32 GetFileSize();
private:
	FileMode::Mode mode;
	FILE* file;
};
