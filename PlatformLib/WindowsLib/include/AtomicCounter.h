#ifndef __ATOMICCOUNTER_H__
#define __ATOMICCOUNTER_H__

#include <windows.h>
#include "revTypedef.h"

class AtomicCounter
{
public:
	AtomicCounter() :
		counter(0)
	{}

	uint32 Increment() { return InterlockedIncrement(&counter); }
	uint32 Decrement() { return InterlockedDecrement(&counter); }
	uint32 GetValue() const { return counter; }

private:
	uint32 counter;

};

#endif
