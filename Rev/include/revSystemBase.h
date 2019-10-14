#ifndef __REVSYSTEMBASE_H__
#define __REVSYSTEMBASE_H__

#include "revSingleton.h"

template<typename T>
class revSystemBase : public revSingleton<T>
{
public:
	virtual void StartUp() = 0;
	virtual void ShutDown() = 0;
};


#endif