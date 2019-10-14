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
	virtual void Open(const char* path, FileMode::Mode mode) = 0;
	virtual void Close() = 0;

	virtual uint8* GetData() = 0;
	virtual uint32 GetFileSize() = 0;
};
