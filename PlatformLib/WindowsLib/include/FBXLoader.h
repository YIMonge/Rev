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
	void LoadStaticMesh(FbxNode* parentNode);
	
	int32 sdkMajorVersion;
	int32 sdkMinorVersion;
	int32 sdkRevision;
	
	FbxManager *manager;
};

#endif

#endif