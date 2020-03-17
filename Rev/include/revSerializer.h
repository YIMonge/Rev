#ifndef __REVSERIALIXER_H__
#define __REVSERIALIXER_H__

#include "cereal/cereal.hpp"
// TODO: only debug if build mode is relase should use binary
#include "cereal/archives/json.hpp"
#include "File.h"
#include "revString.h"
#include <sstream>
//TODO: set custom allocator
using revStringStream = std::stringstream;

#define SERIALIZE_FUNC() template<typename T>\
     void serialize(T& archive)

#define REV_MAKE_NVP(N, T) ::cereal::make_nvp(#N, T)
#define REV_NVP(T) ::cereal::make_nvp(#T, T)

class revSerializer
{
public:
    template<typename T>
    static void Serialize(const char* path, T& obj)
    {
        revStringStream ss;
        {
            cereal::JSONOutputArchive outputArchive(ss);
            outputArchive(REV_MAKE_NVP(root, obj));
        }
        revString outputString = ss.str();
        // TODO: Debug json, Release Binary
        File file(path, FileMode::WriteText);
        file.WriteData(&outputString[0], outputString.size());
        file.Close();
    }

    template<typename T>
    static void Deserialize(const char* path, T& obj)
    {
        revString str;
        File file(path, FileMode::ReadText);
        str.resize(file.GetFileSize());
        file.ReadData(&str[0]);
        file.Close();
        revStringStream ss(str.c_str());
        {
            cereal::JSONInputArchive archive_in(ss);
            archive_in(obj);
        }
    }
};


//-----------------------------------------------------------------------
// Serialize Vector class
//-----------------------------------------------------------------------
#include "revMath.h"
template<typename T>
void serialize(T& archive, revVector2& v)
{
    archive(REV_MAKE_NVP(x, v.x), REV_MAKE_NVP(y, v.y));
}

template<typename T>
void serialize(T& archive, revVector3& v)
{
    archive(REV_MAKE_NVP(x, v.x), REV_MAKE_NVP(y, v.y), REV_MAKE_NVP(z, v.z));
}

template<typename T>
void serialize(T& archive, revVector4& v)
{
    archive(REV_MAKE_NVP(x, v.x), REV_MAKE_NVP(y, v.y), REV_MAKE_NVP(z, v.z), REV_MAKE_NVP(w, v.w));
}

#include "revColor.h"


template<typename T>
void serialize(T& archive, revColor& c)
{
    archive(REV_MAKE_NVP(r, c.r), REV_MAKE_NVP(g, c.g), REV_MAKE_NVP(b, c.b), REV_MAKE_NVP(a, c.a));
}




#endif
