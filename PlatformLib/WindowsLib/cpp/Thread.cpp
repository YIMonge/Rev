#include "Thread.h"
#include <process.h> 

Thread::Thread() 
	: handle(0)
	, stack_size(1024 * 1024)
	, priority(0)
	, revThread()
{}

Thread::~Thread()
{}

void Thread::Create(uint32 priority, uint32 stack_size, revAsyncable* func)
{
	this->priority = priority;
	this->stack_size = stack_size;
	this->SetFunc(func);

	handle = (HANDLE)::_beginthreadex(
		0,
		stack_size,
		ThreadFunc,
		(void*)this,
		0,
		&id
		);
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
	while (state != STATE::DESTROYED) {
		Sleep(1);
	}

}

void Thread::SetFunc(revAsyncable* func)
{
	this->func = func;
}

uint32 __stdcall Thread::ThreadFunc(void* thread)
{
	if (thread != nullptr) {
		Thread* t = (Thread*)thread;
		t->ThreadFuncImpl();
	}
	_endthreadex(0);
	return 0;
}

void Thread::ThreadFuncImpl()
{
	state = STATE::RUNNING;

	while (state == STATE::RUNNING || state == STATE::SUSPEND) {
		while (state == STATE::SUSPEND) {
			Sleep(1);
			continue;
		}

		if (func != nullptr) {
			if (func->AsyncFunc()) {
				break;
			}
		}
		Sleep(1);
	}

	state = STATE::DESTROYED;
}

