#include <android_native_app_glue.h>
#include "File.h"
#include "AndroidContext.h"

File::File() :
file(nullptr),
length(0)
{
}

File::~File()
{
}


bool File::Open(const char* path, FileMode mode)
{
    this->mode = mode;
    file = AAssetManager_open(appContext->activity->assetManager,
            path, AASSET_MODE_BUFFER);
    // TODO: assertion
    if(file == nullptr) return false;
    length = AAsset_getLength(file);
    return true;
}

void File::Close()
{
    if(file == nullptr) return;
    AAsset_close(file);
    file = nullptr;
}

void File::ReadData(uint8* data, uint32 length)
{
    length = length == 0 ? this->length : length;
    AAsset_read(file, data, length);
}

uint32 File::GetFileSize()
{
    return length;
}