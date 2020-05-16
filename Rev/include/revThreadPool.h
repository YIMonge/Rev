#ifndef __REVTHREADPOOL_H__
#define __REVTHREADPOOL_H__

#include "revTypedef.h"
#include "revQueue.h"
#include "revThread.h"
#include "revArray.h"
#include "revAsyncable.h"
#include "revSingleton.h"

class revThreadPool : public revSingleton<revThreadPool>
{
public:
	struct WorkerInfo
	{
		WorkerInfo(uint32 _priority, uint32 _stack_size, uint32 _max_work_num)
			: priority(_priority)
			, stack_size(_stack_size)
			, max_work_num(_max_work_num)
		{}
		uint32 priority;
		uint32 stack_size;
		uint32 max_work_num;
	};

	revThreadPool(const WorkerInfo& info, uint32 worker_num);

	void Suspend();
	void Resume();
	void Join();

	void AddWork(Function work);	

private:
	class Worker : public revAsyncable
	{
	public:
		Worker()
        {
        }

        virtual ~Worker()
        {
		}

		void Init(const WorkerInfo& info)
		{
			job_queue = revQueue<Function>(info.max_work_num);
		}

		virtual bool AsyncFunc()
		{
			return true;
		}

	private:
		revThread* thread;
		revQueue<Function> job_queue;
	};


	revArray<Worker> workers;
};

#endif