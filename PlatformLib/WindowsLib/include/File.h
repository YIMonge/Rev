#pragma once

#include "revFile.h"
#include <stdio.h>
#include <sys/stat.h>

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
	virtual uint32 ReadData(void* data, uint32 length = 0, uint32 offset = 0);
	virtual void WriteData(const void* data, uint32 length);
	void WriteAppend(const void* data, uint32 length);
	virtual uint32 GetFileSize();

	static revString GetDirectoryPath(const revString& path)
	{
		revString result = path;
		struct stat st;
		if (stat(result.c_str(), &st) != 0) {
			return  "";
		}

		// isDirectory
		if ((st.st_mode & S_IFMT) == S_IFDIR) {
			return result;
		}

		return result.substr(0, path.find_last_of('/'));
	}

private:
	FileMode mode;
	FILE* file;
};
