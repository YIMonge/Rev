#ifndef __REVRECT_H__
#define __REVRECT_H__

#include "revTypedef.h"

class revRect
{
public:
	revRect(uint32 w, uint32 h) {
		this->w = w;
		this->h = h;
	}
	uint32 w;
	uint32 h;
};


#endif

