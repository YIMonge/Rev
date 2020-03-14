#include "../include/File.h"
#include <assert.h>

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

File::~File()
{
}
	
bool File::Open(const char* path, FileMode mode)
{
	this->mode = mode;
	file = fopen(path, FileModeString[static_cast<int>(mode)]);
	assert(file != nullptr);
	return file != nullptr;

}
	
void File::Close()
{
	assert(file != nullptr);
	fclose(file);
	file = nullptr;
}

void File::GetData(char* data, uint32 length)
{
	if (length == 0) length = GetFileSize();
	fread(data, 1, length, file);
}

uint32 File::GetFileSize()
{
	assert(file != nullptr);
	fseek(file, 0, SEEK_END);
	uint32 size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}