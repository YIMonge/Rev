#ifndef __FILE_H__
#define __FILE_H__

#include "android/asset_manager.h"
#include "revArray.h"
#include "revFile.h"

class File : public revFile
{
public:
    File();
    File(const char* path, FileMode mode);
    virtual ~File();

    virtual bool Open(const char* path, FileMode mode);
    virtual void Close();
    virtual void ReadData(char* data, uint32 length = 0);
    virtual void WriteData(char* data, uint32 length);
    virtual uint32 GetFileSize();

private:
    AAsset* file;
    uint32 length;
    FileMode mode;
};

#endif
