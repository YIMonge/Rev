#ifndef __REVRESOURCE_H__
#define __REVRESOURCE_H__

#include "revSerializer.h"
#include "revTypedef.h"
#include "revArray.h"
#include "revString.h"
#include "revHash.h"
#include "AtomicCounter.h"

class revResource
{
public:
	revResource() {}
	virtual ~revResource() {}

	const revString& GetName() const { return name; }
	uint64 GetUUID() const { return uuid; }

	void SetName(const revString& name) { this->name = name; }
	void SetName(const char* name) { this->name = name; }
	void SetFilePath(const revString& filePath)
	{
		this->filePath = filePath;
		uuid = revHash<const char*>()(filePath.c_str());
	}
	void SetFilePath(const char* filePath)
	{
		this->filePath = filePath;
		uuid = revHash<const char*>()(filePath);
	}
	const char* GetFilePath() const { return filePath.c_str(); }

	static const uint32 MAX_META_PATH_LENGTH = 256;
	static void MakeMetaPath(char* metaPath, const char* path)
	{
		strcpy(metaPath, path);
		strcat(metaPath, ".meta");
	}

	virtual void Destroy() {};

	
	SERIALIZE_FUNC()
	{
		archive(REV_NVP(uuid),
			REV_NVP(userData));
	}
	

private:
	uint64 uuid;
	revString name;
	revString filePath;
	revString userData;

	mutable AtomicCounter refCount;
};

#endif