#include "Thread.h"
#include <unistd.h>

Thread::Thread() 
	: handle(0)
	, stack_size(1024 * 1024)
	, priority(0)
{}

Thread::~Thread()
{}

void Thread::Create(uint32 priority, uint32 stack_size, IAsyncable* func)
{
	this->priority = priority;
	this->stack_size = stack_size;

	this->SetFunc(func);

	pthread_create(&handle , NULL , ThreadFunc , this);
}

void Thread::Destroy()
{
	state = STATE::WAIT_FOR_DESTROY;
}

void Thread::Suspend()
{
	state = STATE::SUSPEND;
}

void Thread::Resume()
{
	if (state == STATE::SUSPEND) {
		state = STATE::RUNNING;
	}
}

void Thread::Join()
{
	// TODO	
}

void Thread::SetFunc(IAsyncable* func)
{
	this->func = func;
}

void* Thread::ThreadFunc(void* thread)
{
	if (thread != nullptr) {
		Thread* t = (Thread*)thread;
		t->ThreadFuncImpl();
	}
	return 0;
}

void Thread::ThreadFuncImpl()
{
	state = STATE::RUNNING;

	while (state == STATE::RUNNING || state == STATE::SUSPEND) {
		while (state == STATE::SUSPEND) {
			sleep(1);
			continue;
		}

		if (func != nullptr) {
			if (func->AsyncFunc()) {
				break;
			}
		}
		sleep(1);
	}

	state = STATE::DESTROYED;
}

