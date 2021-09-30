#include"iThreadPool.h"
#include"iStd.h"

iThreadPool* iThreadPool::S = NULL;

void* work(void* info)
{
	iThreadInfo* threadInfo = (iThreadInfo*)info;

	while (1)
	{
		pthread_mutex_lock(&threadInfo->condMutex);
		threadInfo->status = WORKER_STATUS_WAIT;
		pthread_cond_wait(&threadInfo->cond, &threadInfo->condMutex);
		pthread_mutex_unlock(&threadInfo->condMutex);

		threadInfo->status = WORKER_STATUS_WORKING;
		iJobInfo* jobInfo = &threadInfo->jobInfo;

		if (jobInfo->flag == iThreadJobFlagNoArg)
		{
			((ThreadJobMethodNoArg)jobInfo->job)();
		}
		else if (jobInfo->flag == iThreadJobFlagOneArg)
		{
			((ThreadJobMethodOneArg)jobInfo->job)(jobInfo->args[0]);
		}
		else if (jobInfo->flag == iThreadJobFlagTwoArg)
		{
			((ThreadJobMethodTwoArg)jobInfo->job)(jobInfo->args[0], jobInfo->args[1]);
		}
	}

	return NULL;
}

iThreadPool::iThreadPool()
	:job(MAX_JOB_QUEUE_SIZE)
{
	workerNum = MAX_THREAD_NUM;
	worker = new iThreadInfo[workerNum];

	for (int i = 0; i < workerNum; i++)
	{
		iThreadInfo* info = &worker[i];

		info->threadID = i;
		info->jobInfo.args.resize(iThreadJobFlagMax - 1);

		pthread_mutex_init(&info->condMutex, NULL);
		pthread_cond_init(&info->cond, NULL);

		pthread_create(&info->thread, NULL, work, (void*)info);
	}
}

iThreadPool::~iThreadPool()
{
	for (int i = 0; i < workerNum; i++)
	{
		pthread_mutex_destroy(&worker[i].condMutex);
		pthread_cond_destroy(&worker[i].cond);
		pthread_kill(worker[i].thread, 0);
	}

	delete[] worker;
}

iThreadPool* iThreadPool::share()
{
	if (!S) S = new iThreadPool();
	return S;
}

void iThreadPool::addJob(ThreadJobMethodNoArg method)
{
	if (job.num == job.size)
	{
		printf("your order was skipped because job queue size is small than your orders. increase your queue size.\n");
		return;
	}

	iJobInfo* info = new iJobInfo;
	info->flag = iThreadJobFlagNoArg;
	info->job = method;

	job.push(info);
}

void iThreadPool::addJob(ThreadJobMethodOneArg method, void* arg)
{
	if (job.num == job.size)
	{
		printf("your order was skipped because job queue size is small than your orders. increase your queue size.\n");
		return;
	}

	iJobInfo* info = new iJobInfo;
	info->flag = iThreadJobFlagOneArg;
	info->job = method;
	info->args.resize(1);
	info->args.push_back(arg);

	job.push(info);
}

void iThreadPool::addJob(ThreadJobMethodTwoArg method, void* arg1, void* arg2)
{
	if (job.num == job.size)
	{
		printf("your order was skipped because job queue size is small than your orders. increase your queue size.\n");
		return;
	}

	iJobInfo* info = new iJobInfo;
	info->flag = iThreadJobFlagTwoArg;
	info->job = method;
	info->args.resize(2);
	info->args.push_back(arg1);
	info->args.push_back(arg2);

	job.push(info);
}

void iThreadPool::update()
{
	if (job.empty()) return;

	for (int i = 0; i < workerNum; i++)
	{
		iThreadInfo* info = &worker[i];

		if (info->status == WORKER_STATUS_WAIT)
		{
			iJobInfo* jobInfo = (iJobInfo*)job.pop();

			info->jobInfo = *jobInfo;
			delete jobInfo;

			pthread_mutex_lock(&info->condMutex);
			pthread_cond_signal(&info->cond);
			pthread_mutex_unlock(&info->condMutex);
			return;
		}
	}

	printf("thread count are small than your orders. recommend to increase your thread count.\n");
}

