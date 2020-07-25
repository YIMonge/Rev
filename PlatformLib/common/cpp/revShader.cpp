#include "revShader.h"
#include "Log.h"

bool revShader::LoadMetaData(const char* path)
{
	char metaPath[MAX_META_PATH_LENGTH];
	MakeMetaPath(metaPath, path);
#ifdef _DEBUG
	File metaFile;
	// TODO: debug always recreate.... should compare last modified 
	if (!metaFile.Open(metaPath, FileMode::ReadText)) {
		NATIVE_LOGE("shader meta file is not found : %s", metaPath);
		return false;
	}
	else metaFile.Close();
#endif
	revSerializer::Deserialize(metaPath, *this);
	return true;
}