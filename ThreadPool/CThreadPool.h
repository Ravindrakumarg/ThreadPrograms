
#pragma once

#include "CThread.h"

#define MAX_THREAD_COUNT	5

class CThreadPool
{
public:
	CThreadPool(DWORD pThreadCount);
	~CThreadPool();

public:
	DWORD AssignWork(const char* pMessage);

private:
	DWORD	InitializePool(DWORD pThreadCount);
	DWORD	GetAvailableThreadID();
	DWORD	GetThreadCount();


private:
	CThread	**thread_pool;
	HANDLE	thread_handle[MAX_THREAD_COUNT];
	DWORD	PoolSize;
};