#pragma once

#include "revFile.h"
#include <stdio.h>

#ifdef  _DEBUG
#define RESOURCE_PATH "../../Resources/"
#elif defined(_RELEASE)
#define RESOURCE_PATH "Resources/"
#endif //  _DEBUG

class File : public revFile
{
public:
	File();
	File(const char* path, FileMode mode);
	virtual ~File();

	virtual bool Open(const char* path, FileMode mode);
	virtual void Close();
	virtual void ReadData(char* data, uint32 length = 0);
	virtual void WriteData(char* data, uint32 length);
	virtual uint32 GetFileSize();
	static bool isExist(const char* path);

private:
	FileMode mode;
	FILE* file;
};
