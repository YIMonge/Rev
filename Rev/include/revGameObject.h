#ifndef __REVGAMEOBJECT_H__
#define __REVGAMEOBJECT_H__

#include "revArray.h"

class revGameObject
{
public:
	revGameObject()
	{		
	}
	virtual ~revGameObject(){}


	virtual void Update() {}
	virtual void SyncUpdate() {}

};

#endif
