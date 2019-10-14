#ifndef __REVSHADER_H__
#define __REVSHADER_H__

#include "revString.h"
#include "TypeDef.h"

class revShader
{
public:
	revShader();
	~revShader();
	
private:
	revString name;
	uint32    id;
};

#endif