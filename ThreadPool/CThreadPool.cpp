#include "CThreadPool.h"

CThreadPool::CThreadPool(DWORD pThreadCount)
{
	if (InitializePool(pThreadCount) < 0)
	{
		std::cout << "Intialization of Thread pool failed" << std::endl;
	}
}

CThreadPool::~CThreadPool()
{
	for (DWORD counter = 0; counter < PoolSize; ++counter)
	{
		delete thread_pool[counter];
	}
}

DWORD CThreadPool::AssignWork(const char * pMessage)
{
	DWORD threadID = GetAvailableThreadID();
	if (threadID < 0)
		return -1;

	return thread_pool[threadID]->SetMessage(pMessage);
}

DWORD CThreadPool::InitializePool(DWORD pThreadCount)
{
	PoolSize = 0;

	if (pThreadCount > MAX_THREAD_COUNT)
	{
		std::cout << "Max thread allowed is " << MAX_THREAD_COUNT << std::endl;
		return -1;
	}

	for (DWORD counter = 0; counter < pThreadCount; ++counter)
	{
		thread_pool[counter] = new CThread();
		thread_pool[counter]->InitialiseThread(counter);
		thread_handle[counter] = thread_pool[counter]->GetThreadHandle();
	}

	PoolSize = pThreadCount;

	DWORD waitStatus = WaitForMultipleObjects(GetThreadCount(), thread_handle, TRUE, INFINITE);

	switch (waitStatus)
	{
	case WAIT_OBJECT_0:
		std::cout << "all threads closed successfully" << std::endl;
		break;

	default:
		std::cout << "wait status failed. error:" << GetLastError() << std::endl;
		return -1;
	}

	return 0;
}

DWORD CThreadPool::GetAvailableThreadID()
{
	DWORD counter;
	for (counter = 0; counter < PoolSize; ++counter)
	{
		if (!thread_pool[counter]->IsThreadFree())
			continue;
	}

	if (counter == PoolSize)
	{
		std::cout << "Thread Pool is completely occupied" << std::endl;
		return -1;
	}

	return counter;
}

DWORD CThreadPool::GetThreadCount()
{
	return PoolSize;
}
