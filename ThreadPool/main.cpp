
#include "CThreadPool.h"

const char *msg[] = { {"message1"},
					{"message2"},
					{"message3"},
					{"message4"},
					{"message5"} };

int main()
{
	CThreadPool pool(5);

	for (DWORD counter = 0; counter < 5; ++counter)
	{
		pool.AssignWork(msg[counter]);
	}

	return 0;
}
