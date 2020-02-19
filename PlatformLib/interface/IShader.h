#ifndef __ISHADER_H__
#define __ISHADER_H__

#include "revString.h"
#include "revTypedef.h"

class IShader
{
public:
	IShader() :
	name("")
	{
	}
	virtual ~IShader(){}
	
private:
	revString name;
};

#endif