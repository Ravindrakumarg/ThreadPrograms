
#pragma once

#include <iostream>
#include <Windows.h>

#define FILE_NAME	"d:/testpool.txt"


class CThread
{
public:
	CThread();
	~CThread();

public:
	DWORD	InitialiseThread(DWORD);
	DWORD	GetThreadID();
	DWORD	Shutdown();
	BOOL	IsThreadFree();
	DWORD	SetMessage(const char* pMessage);
	HANDLE	GetThreadHandle();

private:
static	DWORD	WINAPI ThreadFunc(LPVOID);
	DWORD	Run();
	void	CleanUp();
	DWORD	SetActive();

private:
	HANDLE		ThreadHandle;
	BOOL		isFree;
	HANDLE		ThreadEvent[2];
	char *		message;
	DWORD		ThreadID;
};