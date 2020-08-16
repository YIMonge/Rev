#include "SampleScene.h"

SampleScene::SampleScene()
{
}

SampleScene::~SampleScene()
{
}

void SampleScene::Load()
{
	shaderResources.push_back("ironman_vert");
	shaderResources.push_back("ironman_frag");
	textureResources.push_back("sample_tex.png");
	modelResources.push_back("Models/ironman.mdl");
	revScene::Load();

	revGraphics& graphics = revGraphics::Get();
 	meshRenderer = graphics.CreateMeshRenderer();

}

void SampleScene::Destroy()
{

}
