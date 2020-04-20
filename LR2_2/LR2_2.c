#include <stdio.h>
#include <conio.h>
#include <windows.h>

#pragma argsused

#define CREATE_SUSPENDED 0x00000004

long int ii1;

#define MAX_SEM_COUNT 10   

HANDLE gh_semaphore;

DWORD WINAPI my_thread1(LPVOID lp_parameter)
{
	BOOL* active = (BOOL*)lp_parameter;
	*active = TRUE;

	for (ii1 = 0; ii1 < 10; ii1++)
	{
		const DWORD dw_wait_result = WaitForSingleObject(gh_semaphore, 0);
		switch (dw_wait_result)
		{
		case WAIT_OBJECT_0:
			printf("Thread %lx: wait succeeded\n", GetCurrentThreadId());
			Sleep(555);
			break;

		case WAIT_TIMEOUT:
			printf("Thread %lx: wait timed out\n", GetCurrentThreadId());
			break;
		default:;
		}
	}
	*active = FALSE;
	ExitThread(0);
}
long int ii2;
DWORD WINAPI my_thread2(LPVOID lp_parameter)
{
	BOOL* active = (BOOL*)lp_parameter;
	*active = TRUE;  

	for (ii2 = 0; ii2 < 10; ii2++)
	{
		const DWORD dw_wait_result = WaitForSingleObject(gh_semaphore, 0);   

		switch (dw_wait_result)
		{
		case WAIT_OBJECT_0:
			printf("Thread %lx: wait succeeded\n", GetCurrentThreadId());
			Sleep(755);
			break;

		case WAIT_TIMEOUT:
			printf("Thread %lx: wait timed out\n", GetCurrentThreadId());
			break;
		default:;
		}
	}

	*active = FALSE;  
	ExitThread(0);   
}


void init_thread(void)
{
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	const int nmb_prc = system_info.dwNumberOfProcessors;

	DWORD thread_id1;
	DWORD thread_id2;
	BOOL active1 = TRUE;
	BOOL active2 = TRUE;

	HANDLE h_thread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread1, (void*)&active1, CREATE_SUSPENDED, &thread_id1);
	HANDLE h_thread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread2, (void*)&active2, CREATE_SUSPENDED, &thread_id2);

	SetThreadAffinityMask(h_thread1, 1L);// use CPU 0 only
	SetThreadAffinityMask(h_thread2, 2L);// use CPU 1 only
	if (!SetThreadPriority(h_thread1, THREAD_PRIORITY_HIGHEST)) printf("\nSetPriority error\n");
	if (!SetThreadPriority(h_thread2, THREAD_PRIORITY_HIGHEST)) printf("\nSetPriority error\n");

	HANDLE h_process = GetCurrentProcess();
	DWORD dw_process_affinity_mask;
	DWORD dw_system_affinity_mask;
	GetProcessAffinityMask(h_process, &dw_process_affinity_mask, &dw_system_affinity_mask);

	printf("\nThreads IDs %lu %lu\n", thread_id1, thread_id2);

	ResumeThread(h_thread1);
	ResumeThread(h_thread2);
	const int thread_priority1 = GetThreadPriority(h_thread1);
	const int thread_priority2 = GetThreadPriority(h_thread2);
	printf("\nHere I am! nmb_prc=%d (thread_priority1 %d thread_priority2 %d): %lx, %lx\n",
		nmb_prc, thread_priority1, thread_priority2, dw_process_affinity_mask, dw_system_affinity_mask);

	while (active1 == TRUE || active2 == TRUE)
	{
		if (_kbhit()) {
			_getch();
			ReleaseSemaphore(gh_semaphore, 1, NULL);
		}
	}

	CloseHandle(h_thread1);
	CloseHandle(h_thread2);
	CloseHandle(gh_semaphore);
}
int main(int argc, char* argv[])
{
	gh_semaphore = CreateSemaphore(NULL, MAX_SEM_COUNT, MAX_SEM_COUNT, NULL);

	if (gh_semaphore == NULL)
	{
		printf("CreateSemaphore error: %lu\n", GetLastError());
		return 1;
	}

	init_thread();
	_getch();
	return 0;
}
