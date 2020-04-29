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

protected:
	void makeMetaPath(char* metaPath, const char* path)
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