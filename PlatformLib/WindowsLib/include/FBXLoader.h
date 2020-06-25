#ifndef __FBXLOADER_H__
#define __FBXLOADER_H__

#ifdef  _DEBUG
#include <fbxsdk.h>
#include "revArray.h"
#include "revString.h"
#include "revMesh.h"

class FBXLoader
{
public:
	FBXLoader();
	virtual ~FBXLoader();
	void LoadFromFile(const revString& path, revMesh* mesh);

private:
	void ImportNode(FbxNode* node);
	void LoadStaticMesh(FbxNode* parentNode);
	
	int32 sdkMajorVersion;
	int32 sdkMinorVersion;
	int32 sdkRevision;
	
	FbxManager *manager;
};

#endif

#endif