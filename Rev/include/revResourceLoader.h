#ifndef __REVRESOURCELOADER_H__
#define __REVRESOURCELOADER_H__

template<typename T>
class revResourceLoader
{
public:
	revResourceLoader() {}
	virtual ~revResourceLoader(){}

	virtual bool LoadFromFile(const char* path, T* resource)
	{
		if (resource == nullptr) return false;

		resourcePath = RESOURCE_PATH;
		resourcePath += path;
		resource->SetFilePath(path);

		if (!LoadMetaFile(resourcePath.c_str(), resource))	return false;
		if (!file.Open(resourcePath.c_str(), FileMode::ReadBinary)) {
			return OnLoadFailed(resource);
		}

		return true;
	}


protected:
	virtual bool OnLoadFailed(T* resource)
	{
		return false;
	}


	virtual bool LoadMetaFile(const char* path, T* resource)
	{
		char metaPath[revResource::MAX_META_PATH_LENGTH];
		revResource::MakeMetaPath(metaPath, path);
#ifdef _DEBUG
		File metaFile;
		if (!metaFile.Open(metaPath, FileMode::ReadText)) {
			revSerializer::Serialize(metaPath, *resource);
			return true;
		}
		else metaFile.Close();
#endif
		return revSerializer::Deserialize(metaPath, *resource);

		return true;
	}

	File file;
	revString resourcePath;
};

#endif
