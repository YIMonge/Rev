#include "revShader.h"

bool revShader::LoadMetaData(const char* path)
{
	char metaPath[256];
	makeMetaPath(metaPath, path);
#ifdef _DEBUG
	File metaFile;
	// TODO: debug always recreate....
	if (!metaFile.Open(metaPath, FileMode::ReadText) || true) {
		return false;
	}
	else metaFile.Close();
#endif
	revSerializer::Deserialize(metaPath, metaData);
	return true;
}