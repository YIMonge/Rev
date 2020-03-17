#ifndef __FILE_H__
#define __FILE_H__

#include "android/asset_manager.h"
#include "revArray.h"
#include "revFile.h"

class File : public revFile
{
public:
    File();
    virtual ~File();

    bool Open(const char* path, FileMode mode);
    void Close();
    void ReadData(uint8* data, uint32 length = 0);
    uint32 GetFileSize();

private:
    AAsset* file;
    uint32 length;
    FileMode mode;
};

#endif
