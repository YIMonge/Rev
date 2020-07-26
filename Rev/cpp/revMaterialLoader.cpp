#include "revMaterialLoader.h"

const uint32 DATA_VERSION = 1;

revMaterialLoader::revMaterialLoader()
{
}

revMaterialLoader::~revMaterialLoader()
{}

bool revMaterialLoader::LoadFromFile(const char* path, revMaterial* material)
{
	if (!revResourceLoader::LoadFromFile(path, material)) return false;

	uint32 offset = 0;
	uint32 dataVer;
	offset = file.ReadData(&dataVer, sizeof(uint32), offset);
	if (dataVer != DATA_VERSION) {
		// if need upgrade, write here
		file.Close();
		return false;
	}
	uint32 propertyCount;
	offset = file.ReadData(&propertyCount, sizeof(uint32), offset);
	
	material->properties.resize(propertyCount);
	for (uint32 i = 0; i < propertyCount; ++i) {
		offset = file.ReadData(&material->properties[i], sizeof(revMaterial::Property), offset);
	}
	file.Close();
	return true;
}

#ifdef _WINDOWS
void revMaterialLoader::WriteMaterial(const char* path, const revMaterial& material)
{
	DeleteFileA(path);
	File writingFile;
	writingFile.Open(path, FileMode::WriteBinary);
	writingFile.WriteAppend(&DATA_VERSION, sizeof(uint32));
	uint32 propertyCount = static_cast<uint32>(material.properties.size());
	writingFile.WriteAppend(&propertyCount, sizeof(uint32));
	for (uint32 i = 0; i < propertyCount; ++i) {
		writingFile.WriteAppend(&material.properties[i], sizeof(revMaterial::Property));
	}
	writingFile.Close();
}
#endif
