#include "GameCharacter.h"
#include "revGraphics.h"

GameCharacter::GameCharacter()
{
	meshRenderer = revGraphics::Get().CreateMeshRenderer();
	// TODO:レンダラーの初期化と描画コマンドの作成を作っていく
}


GameCharacter::~GameCharacter()
{

}