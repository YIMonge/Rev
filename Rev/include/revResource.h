#ifndef __REVRESOURCE_H__
#define __REVRESOURCE_H__

#include "revSerializer.h"
#include "revTypedef.h"
#include "revArray.h"
#include "revString.h"

class revResource
{
public:
	revResource() {}
	virtual ~revResource() {}

	const revString& GetName() const { return name; }

	void SetName(const revString& name) { this->name = name; }
	void SetName(const char* name) { this->name = name; }

	static const uint32 MAX_META_PATH_LENGTH = 256;
	static void MakeMetaPath(char* metaPath, const char* path)
	{
		strcpy(metaPath, path);
		strcat(metaPath, ".meta");
	}
private:
	revString name;
};

struct DefaultMetaData
{
	uint32 guid;
	revString userData;
};

SERIALIZE_FUNC_NON_INTRUSIVE(DefaultMetaData, data)
{
	archive(REV_MAKE_NVP(guid, data.guid),
		REV_MAKE_NVP(userData, data.userData));
}
#endif