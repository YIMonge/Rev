#ifndef __THREAD_H__
#define __THREAD_H__

#include "revThread.h"
#include <pthread.h>

class Thread : public revThread
{
public:
	Thread();
	virtual ~Thread();

	virtual void Create(uint32 priority, uint32 stack_size, revAsyncable* func);
	virtual void Destroy();
	virtual void Suspend();
	virtual void Resume();
	virtual void Join();
	virtual void SetFunc(revAsyncable* func);

protected:
	static void* ThreadFunc(void* thread);
	void ThreadFuncImpl();

	pthread_t 	handle;
	uint32 		stack_size;
	uint32 		id;
	uint32 		priority;
	revAsyncable* func;
};

#endif