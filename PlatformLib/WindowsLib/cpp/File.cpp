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
	
void File::Open(const char* path, FileMode::Mode mode)
{
	this->mode = mode;
	file = fopen(path, FileModeString[static_cast<int>(mode)]);
	assert(file != nullptr);
}
	
void File::Close()
{
	assert(file != nullptr);
	fclose(file);
	file = nullptr;
}

uint32 File::GetFileSize()
{
	assert(file != nullptr);
	fseek(file, 0, SEEK_END);
	uint32 size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}