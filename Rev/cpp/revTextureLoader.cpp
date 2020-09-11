#include "revTextureLoader.h"
#include "revGraphics.h"

#define STB_IMAGE_IMPLEMENTATION
// TODO: set memory allocation way
#define STBI_MALLOC(sz)           malloc(sz)
#define STBI_REALLOC(p,newsz)     realloc(p,newsz)
#define STBI_FREE(p)              free(p)
#include "stb_image.h"


revTextureLoader::revTextureLoader()
{

}

revTextureLoader::~revTextureLoader()
{

}

bool revTextureLoader::LoadFromFile(const char* path, revTexture* texture)
{
	if (texture == nullptr) return false;
	if (!revResourceLoader::LoadFromFile(path, texture)) return false;

	uint32 offset = 0;	
	uint32 fileLength = file.GetFileSize();
	int8* data = new int8[fileLength];
	file.ReadData(data);
	file.Close();

	int32 n;
	uint32 width, height;
	uint8* imageData = stbi_load_from_memory(reinterpret_cast<uint8*>(data), fileLength, reinterpret_cast<int32*>(&width), reinterpret_cast<int32*>(&height), &n, 4);
	texture->width = width;
	texture->height = height;
	texture->rawData = imageData;

	delete[] data;
	return true;
}

void revTextureLoader::OnLoadComplete(revTexture* texture)
{
	revGraphics::Get().UploadResource(texture);
}
