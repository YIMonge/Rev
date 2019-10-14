#include "../include/Locker.h"

Locker::Locker()
{
	pthread_mutex_init(&mutex, NULL);
}

Locker::~Locker()
{
	pthread_mutex_destroy(&mutex);
}

void Locker::Lock()
{
	pthread_mutex_lock(&mutex);
}

void Locker::Unlock()
{
	pthread_mutex_unlock(&mutex);
}
