#ifndef __FBXLOADER_H__
#define __FBXLOADER_H__

#ifdef  _DEBUG
#include <fbxsdk.h>
#include "revArray.h"
#include "revString.h"
#include "revModel.h"

class FBXLoader
{
public:
	FBXLoader();
	virtual ~FBXLoader();
	bool LoadFromFile(const revString& path, revModel* model);

private:
	void ImportNode(FbxNode* node, revModel* model);
	void ImportVertexData(FbxNode* node, revModel* model);
	void ImportMaterialData(FbxNode* node, revModel* model);


	int32 sdkMajorVersion;
	int32 sdkMinorVersion;
	int32 sdkRevision;
	
	FbxManager *manager;
};

#endif

#endif