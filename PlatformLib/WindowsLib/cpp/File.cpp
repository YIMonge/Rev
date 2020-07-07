#include "File.h"
#include "Log.h"
#include <assert.h>
#include <string.h>

#pragma warning (disable : 4996)

namespace
{
	const char* FileModeString[] =
	{
		"wb",
		"rb",
		"wt",
		"rt",
	};
}

File::File() : 
	file(nullptr)
{
}

File::File(const char* path, FileMode mode)
{
	// TODO: own file database management if I could do I dont need these path replacement. 
	Open(path, mode);
}

File::~File()
{
}
	
bool File::Open(const char* path, FileMode mode)
{
	this->mode = mode;
	file = fopen(path, FileModeString[static_cast<int>(mode)]);
	return file != nullptr;

}
	
void File::Close()
{
	assert(file != nullptr);
	fclose(file);
	file = nullptr;
}

uint32 File::ReadData(void* data, uint32 length, uint32 offset)
{
	if (mode == FileMode::WriteBinary || mode == FileMode::WriteText) {
		NATIVE_LOGE("File mode is write only. cant read data");
		return offset;
	}
	if (length == 0) length = GetFileSize();
	fseek(file, offset, SEEK_SET);
	fread(data, 1, length, file);
	fseek(file, 0, SEEK_SET);
	return offset + length;
}

void File::WriteData(const void* data, uint32 length)
{
	if (mode == FileMode::ReadBinary || mode == FileMode::ReadText) {
		NATIVE_LOGE("File mode is read only. cant write data");
		return;
	}
	fwrite(data, 1, length, file);
}

void File::WriteAppend(const void* data, uint32 length)
{
	if (mode == FileMode::ReadBinary || mode == FileMode::ReadText) {
		NATIVE_LOGE("File mode is read only. cant write data");
		return;
	}

	fseek(file, 0, SEEK_END);
	fwrite(data, 1, length, file);
	fseek(file, 0, SEEK_SET);
}


uint32 File::GetFileSize()
{
	assert(file != nullptr);
	fseek(file, 0, SEEK_END);
	uint32 size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}