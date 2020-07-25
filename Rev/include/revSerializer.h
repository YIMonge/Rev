#ifndef __REVSERIALIXER_H__
#define __REVSERIALIXER_H__

#include "cereal/cereal.hpp"
// TODO: only debug if build mode is relase should use binary
#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/polymorphic.hpp"
#include "File.h"
#include "revString.h"
#include <sstream>
//TODO: set custom allocator
using revStringStream = std::stringstream;

#define SERIALIZE_FUNC() template<class Archive>\
     void serialize(Archive& archive)

#define SERIALIZE_FUNC_NON_INTRUSIVE(type, name) template<class Archive>\
     void serialize(Archive& archive, type& name)

#define SERIALIZE_BASE_CLASS(type) cereal::make_nvp(#type, ::cereal::base_class<type>(this))

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
        file.WriteData(&outputString[0], static_cast<uint32>(outputString.size()));
        file.Close();
    }

    template<typename T>
    static bool Deserialize(const char* path, T& obj)
    {
        revString str;
		File file;
		if (!file.Open(path, FileMode::ReadText)) return false;
        str.resize(file.GetFileSize());
        file.ReadData(&str[0]);
        file.Close();
        revStringStream ss(str.c_str());
        {
            cereal::JSONInputArchive archive_in(ss);
            archive_in(obj);
        }
		return true;
    }
};


//-----------------------------------------------------------------------
// Serialize Vector class
//-----------------------------------------------------------------------
#include "revMath.h"

SERIALIZE_FUNC_NON_INTRUSIVE(revVector2, v)
{
    archive(REV_MAKE_NVP(x, v.x), REV_MAKE_NVP(y, v.y));
}

SERIALIZE_FUNC_NON_INTRUSIVE(revVector3, v)
{
    archive(REV_MAKE_NVP(x, v.x), REV_MAKE_NVP(y, v.y), REV_MAKE_NVP(z, v.z));
}

SERIALIZE_FUNC_NON_INTRUSIVE(revVector4, v)
{
    archive(REV_MAKE_NVP(x, v.x), REV_MAKE_NVP(y, v.y), REV_MAKE_NVP(z, v.z), REV_MAKE_NVP(w, v.w));
}

#include "revColor.h"
SERIALIZE_FUNC_NON_INTRUSIVE(revColor, c)
{
    archive(REV_MAKE_NVP(r, c.r), REV_MAKE_NVP(g, c.g), REV_MAKE_NVP(b, c.b), REV_MAKE_NVP(a, c.a));
}

#endif
