#ifndef __SAMPLESCENE_H__
#define __SAMPLESCENE_H__

#include "revScene.h"
#include "revMeshRenderer.h"

class SampleScene : public revScene
{
public:
	SampleScene();
	virtual ~SampleScene();

	virtual void Load();
	virtual void Destroy();


	revMeshRenderer* meshRenderer;
};

#endif
