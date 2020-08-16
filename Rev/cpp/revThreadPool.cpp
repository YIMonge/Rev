#include "../include/revThreadPool.h"


revThreadPool::revThreadPool(const WorkerInfo& info, uint32 worker_num)
{
	jobQueue = revQueue<Function>(info.max_work_num);


}

void revThreadPool::Suspend()
{
	for(auto worker : workers)
	{

	}
}

void revThreadPool::Resume()
{

}

void revThreadPool::Join()
{

}

void revThreadPool::AddWork(Function work)
{
	criticalSection.Lock();
	jobQueue.Enqueue(work);
	criticalSection.Unlock();
}