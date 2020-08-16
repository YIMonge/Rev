#ifndef __GAMECHARACTER_H__
#define __GAMECHARACTER_H__

#include "revGameObject.h"
#include "revMeshRenderer.h"

class GameCharacter : public revGameObject
{
public:
	GameCharacter();
	virtual ~GameCharacter();

private:
	revMeshRenderer* meshRenderer;

};

#endif
