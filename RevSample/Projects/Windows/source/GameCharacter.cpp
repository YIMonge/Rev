#include "GameCharacter.h"
#include "revGraphics.h"

GameCharacter::GameCharacter()
{
	meshRenderer = revGraphics::Get().CreateMeshRenderer();
	// TODO:�����_���[�̏������ƕ`��R�}���h�̍쐬������Ă���
}


GameCharacter::~GameCharacter()
{

}