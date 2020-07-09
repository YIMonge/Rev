#ifndef __FBXLOADER_H__
#define __FBXLOADER_H__

#ifdef  _DEBUG
#include <fbxsdk.h>
#include "revArray.h"
#include "revString.h"
#include "revModel.h"
#include "revTransform.h"

class FBXLoader
{
public:
	FBXLoader();
	virtual ~FBXLoader();
	bool LoadFromFile(const revString& path, revModel* model);

private:
	void ImportNode(FbxNode* node, revModel* model, revTransform* parent);
	void ImportVertexData(FbxNode* node, revModel* model);
	void ImportMatrix(FbxNode* node, revModel* model, revTransform* parent);
	void ImportMaterialData(FbxNode* node, revModel* model);

	revMatrix44 globalMatrix;
	FbxManager* manager;
	FbxScene* scene;
};

#endif

#endif