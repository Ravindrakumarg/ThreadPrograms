
#include "CThread.h"
#include "fstream"

CThread::CThread()
{
	message = nullptr;
	isFree = TRUE;
}

CThread::~CThread()
{
	CleanUp();
}

DWORD CThread::InitialiseThread(DWORD pThreadID)
{
	ThreadID = pThreadID;

	ThreadEvent[0] = CreateEvent(nullptr, TRUE, FALSE, TEXT("StartThread"));
	ThreadEvent[1] = CreateEvent(nullptr, TRUE, FALSE, TEXT("StopThread"));

	if (ThreadEvent[0] == nullptr || ThreadEvent[1] == nullptr)
	{
		std::cout << "Event creation failed for thread id:" << pThreadID << " with error:" << GetLastError() << std::endl;
	}

	ThreadHandle = CreateThread(nullptr, 0, &ThreadFunc, nullptr, 0, &pThreadID);

	if (ThreadHandle == nullptr)
	{
		std::cout << "Thread Creation failed, threadid:" << pThreadID << " with error:" << GetLastError() << std::endl;
	}

	return 0;
}

DWORD WINAPI CThread::ThreadFunc(LPVOID param)
{
	DWORD waitResult = WaitForMultipleObjects(2, ThreadEvent, TRUE, INFINITE);

	switch (waitResult)
	{
	case WAIT_OBJECT_0:
		Run();
		break;
	case WAIT_OBJECT_0 + 1:
		CloseHandle(ThreadHandle);
		break;
	default:
		std::cout << "wait result failed for thread: " << GetCurrentThreadId() << " with error:" << GetLastError() << std::endl;
		return 0;
	}

	return 1;
}

DWORD CThread::Run()
{
	isFree = false;

	std::ofstream	file_handler;
	file_handler.open(FILE_NAME, std::ios::out | std::ios::app | std::ios::binary);

	file_handler << message;
	file_handler << "by Thread id" << ThreadID;
	file_handler.close();

	return 0;
}

DWORD CThread::SetActive()
{
	if (!SetEvent(ThreadEvent[0]))
	{
		std::cout << "set thread active failed, threadid:" << ThreadID << ", error:" << GetLastError() << std::endl;
		return -1;
	}
	return 0;
}

DWORD CThread::Shutdown()
{
	if (!SetEvent(ThreadEvent[1]))
	{
		std::cout << "set thread to inactive failed, threadid:" << ThreadID << ", error:" << GetLastError() << std::endl;
		return -1;
	}
	return 0;
}

DWORD CThread::GetThreadID()
{
	return ThreadID;
}

BOOL CThread::IsThreadFree()
{
	return isFree;
}

DWORD CThread::SetMessage(const char * pMessage)
{
	message = (char*)malloc(strlen(pMessage));
	memcpy(message, pMessage, strlen(pMessage));

	return SetActive();
}

HANDLE CThread::GetThreadHandle()
{
	return ThreadHandle;
}

void CThread::CleanUp()
{
	if (message)
		delete message;

	CloseHandle(ThreadHandle);
	CloseHandle(ThreadEvent[0]);
	CloseHandle(ThreadEvent[1]);
}

