#include <android_native_app_glue.h>
#include "File.h"
#include "AndroidContext.h"
#include "Log.h"

File::File() :
file(nullptr),
length(0)
{
}

File::File(const char* path, FileMode mode)
{
    Open(path, mode);
}

File::~File()
{
}

bool File::Open(const char* path, FileMode mode)
{
    this->mode = mode;
    file = AAssetManager_open(appContext->activity->assetManager,
            path, AASSET_MODE_BUFFER);
    if(file == nullptr) {
        NATIVE_LOGE("file not found : %s", path);
        return false;
    }
    length = AAsset_getLength(file);
    return true;
}

void File::Close()
{
    if(file == nullptr) return;
    AAsset_close(file);
    file = nullptr;
}

void File::ReadData(char* data, uint32 length)
{
    length = length == 0 ? this->length : length;
    AAsset_read(file, data, length);
}

void File::WriteData(char* data, uint32 length)
{
    // TODO:
}

uint32 File::GetFileSize()
{
    return length;
}