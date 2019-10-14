#ifndef __THREAD_H__
#define __THREAD_H__

#include "IThread.h"
#include <pthread.h>

class Thread : public IThread
{
public:
	Thread();
	virtual ~Thread();		

	virtual void Create(uint32 priority, uint32 stack_size, IAsyncable* func);
	virtual void Destroy();
	virtual void Suspend();
	virtual void Resume();
	virtual void Join();
	virtual void SetFunc(IAsyncable* func);

protected:
	static void* ThreadFunc(void* thread);
	void ThreadFuncImpl();

	pthread_t 	handle;
	uint32 		stack_size;
	uint32 		id;
	uint32 		priority;
	IAsyncable* func;
};

#endif