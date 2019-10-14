#ifndef __LOCKER_H__
#define __LOCKER_H__

#include "../../interface/ILocker.h"
#include <pthread.h>


class Locker : public ILocker
{
public:
	Locker();
	virtual ~Locker();

	virtual void Lock();
	virtual void Unlock();

private:
	pthread_mutex_t mutex;
};

#endif