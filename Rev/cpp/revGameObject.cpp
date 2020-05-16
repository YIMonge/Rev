#include "revGameObject.h"

revGameObject::revGameObject():
	id(0),
	isActive(true),
	isDead(false),
	usingUpdate(false),
	usingLateUpdate(false),
	usingSyncUpdate(false)
{
}

revGameObject::~revGameObject()
{
}