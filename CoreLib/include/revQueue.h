#ifndef __REVQUEUE_H__
#define __REVQUEUE_H__

#include "revArray.h"

template<typename T>
class revQueue
{
public:
	revQueue();
	revQueue(uint32 _capacity);

	bool Enqueue(T& obj);
	T& Dequeue();
	T& Peek() const;
	uint32 Count() const;
	void SetCapacity(uint32 _capacity);

private:
	uint32 first;
	uint32 last;
	uint32 capacity;
	revArray<T> queue;
};

#include "revQueue.inl"

#endif