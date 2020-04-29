#include "../include/File.h"
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

File::File() 
	: file(nullptr)
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

void File::ReadData(char* data, uint32 length)
{
	if (mode == FileMode::WriteBinary || mode == FileMode::WriteText) {
		// TODO: assertion
		return;
	}
	if (length == 0) length = GetFileSize();
	fread(data, 1, length, file);
}

void File::WriteData(char* data, uint32 length)
{
	if (mode == FileMode::ReadBinary || mode == FileMode::ReadText) {
		// TODO: assertion
		return;
	}
	fwrite(data, 1, length, file);
}

uint32 File::GetFileSize()
{
	assert(file != nullptr);
	fseek(file, 0, SEEK_END);
	uint32 size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}