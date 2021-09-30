#pragma once

#ifdef _WIN32
#define HAVE_STRUCT_TIMESPEC
#endif
#include "pthread.h"
#pragma comment(lib, "pthreadVC2.lib")

#include "iType.h"
#include "iQueue.h"
#include "iArray.h"

#define MAX_THREAD_NUM _POSIX_THREAD_THREADS_MAX / 8
#define MAX_JOB_QUEUE_SIZE 50

#define WORKER_STATUS_WAIT	  0
#define WORKER_STATUS_WORKING 1

enum iThreadJobFlag
{
	iThreadJobFlagNoArg = 0,
	iThreadJobFlagOneArg,
	iThreadJobFlagTwoArg,
	iThreadJobFlagMax
};

typedef void (*ThreadJobMethodNoArg)();
typedef void (*ThreadJobMethodOneArg)(void*);
typedef void (*ThreadJobMethodTwoArg)(void*, void*);

struct iThreadInfo;

class iThreadPool
{
private:
	static iThreadPool* S;
	iThreadPool();

public:
	~iThreadPool();
	static iThreadPool* share();

	void addJob(ThreadJobMethodNoArg method);
	void addJob(ThreadJobMethodOneArg method, void* arg);
	void addJob(ThreadJobMethodTwoArg method, void* arg1, void* arg2);

	void update();

private:
	iThreadInfo* worker;
	int workerNum;
	iQueue job;
};

struct iJobInfo
{
	iThreadJobFlag flag;
	void* job;
	iArray args;
};

struct iThreadInfo
{
	uint32 status;

	uint32 threadID;
	pthread_t thread;

	pthread_mutex_t condMutex;
	pthread_cond_t cond;

	iJobInfo jobInfo;
};




