#ifndef __REVFILE_H__
#define __REVFILE_H__

#include "revTypedef.h"

enum class FileMode : uint8
{
	WriteBinary = 0,
	ReadBinary,
	WriteText,
	ReadText,
};

class revFile
{
public:
	virtual ~revFile(){}

	virtual bool Open(const char* path, FileMode mode) = 0;
	virtual void Close() = 0;
	// if length is 0, can get all size of data.
	virtual void ReadData(char* data, uint32 length = 0) = 0;
	virtual void WriteData(char* data, uint32 length) = 0;
	virtual uint32 GetFileSize() = 0;
};

#endif

