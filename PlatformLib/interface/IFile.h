#pragma once

#include "revTypedef.h"

namespace FileMode
{
	enum class Mode : uint8
	{
		WriteBinary = 0,
		ReadBinary,
		WriteText,
		ReadText,
	};
}

class IFile
{
public:
	virtual ~IFile(){}

	virtual bool Open(const char* path, FileMode::Mode mode) = 0;
	virtual void Close() = 0;

	// if length is 0, can get all size of data.
	virtual void GetData(uint8* data, uint32 length = 0) = 0;
	virtual uint32 GetFileSize() = 0;
};
