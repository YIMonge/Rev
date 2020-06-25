#include "revTexture.h"

#define STB_IMAGE_IMPLEMENTATION
// TODO: set memory allocation way
//#define STBI_MALLOC(sz)           malloc(sz)
//#define STBI_REALLOC(p,newsz)     realloc(p,newsz)
//#define STBI_FREE(p)              free(p)
#include "stb_image.h"

bool revTexture::LoadFromFile(revDevice* device, const char* path)
{
    // Load Texture Data
	revString resourcePath(RESOURCE_PATH);
	resourcePath += path;
    File file;
    if (!file.Open(resourcePath.c_str(), FileMode::ReadBinary)) {
        NATIVE_LOGE("failed to open texture : %s", resourcePath.c_str());
        return false;
    }

    // Load Meta Data
    char metaPath[256];
    makeMetaPath(metaPath, resourcePath.c_str());
#ifdef _DEBUG
    // if meta file is not found try to make default meta
    File metaFile;
    if (!metaFile.Open(metaPath, FileMode::ReadText)) {
        NATIVE_LOGI("meta file is not found. create new file : %s", metaPath);
       revSerializer::Serialize(metaPath, metaData);
    }
    else metaFile.Close();
#endif
    revSerializer::Deserialize(metaPath, metaData);
    

    // Load Texture Data
    uint32 fileLength = file.GetFileSize();
    char* data = new char[fileLength];
    file.ReadData(data);
    file.Close();

    int n;
    uint8* imageData = stbi_load_from_memory(reinterpret_cast<uint8*>(data), fileLength, reinterpret_cast<int32*>(&width), reinterpret_cast<int32*>(&height), &n, 4);
    if (!CreateTexture(device, imageData)) {
        stbi_image_free(imageData);
        delete[] data;
        return false;
    }

    stbi_image_free(imageData);
    delete[] data;
    return true;
}