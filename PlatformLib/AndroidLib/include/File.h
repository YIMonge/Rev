#ifndef __FILE_H__
#define __FILE_H__

#include "IFile.h"
#include "android/asset_manager.h"
#include "revArray.h"

class File : public IFile
{
public:
    File();
    virtual ~File();

    bool Open(const char* path, FileMode::Mode mode);
    void Close();
    void GetData(uint8* data, uint32 length = 0);
    uint32 GetFileSize();

private:
    AAsset* file;
    uint32 length;
};

#endif
