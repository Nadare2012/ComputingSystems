#pragma hdrstop
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <windows.h>

#define CREATE_SUSPENDED 0x00000004

time_t t1, t2;
CRITICAL_SECTION lps;
char ok1 = 0, ok2 = 0, ok3 = 0;

BOOL wb_write_to_buffer(const int thread_num)
{
	printf("thread %d waiting critical section\n", thread_num);
	EnterCriticalSection(&lps);
	printf("thread %d entered in critical section\n", thread_num);
	printf("thread %d writing to buffer in critical section\n", thread_num);
	Sleep(10000L);
	printf("thread %d finished writing to buffer in critical section\n", thread_num);
	LeaveCriticalSection(&lps);
	printf("thread %d leaved critical section\n", thread_num);
	return TRUE;
}

BOOL try_wb_write_to_buffer(const int thread_num)
{
	printf("thread %d trying to enter critical section\n", thread_num);
	if (!TryEnterCriticalSection(&lps))
	{
		printf("critical section owned by another thread, thread %d leaving\n", thread_num);
		return FALSE;
	}
	printf("thread %d entered in critical section\n", thread_num);
	printf("thread %d writing to buffer in critical section\n", thread_num);
	Sleep(10000L);
	printf("thread %d finished writing to buffer in critical section\n", thread_num);
	LeaveCriticalSection(&lps);
	printf("thread %d leaved critical section\n", thread_num);
	return TRUE;
}

long int ii1;
float a1, b1, step1 = .001f;
DWORD WINAPI my_thread1(LPVOID lp_parameter)
{
	BOOL* active = (BOOL*)lp_parameter;
	*active = TRUE;
	if (try_wb_write_to_buffer(1)) ok1 = 1;
	for (ii1 = 0; ii1 < 0x1ffffff; ii1++)
	{
		a1 = step1 * (float)ii1;
		b1 = sinf(a1);
	}
	*active = FALSE;
	ExitThread(0);
}

long int ii2;
float a2, b2, step2 = .001f;
DWORD WINAPI my_thread2(LPVOID lp_parameter)
{
	BOOL* active = (BOOL*)lp_parameter;
	*active = TRUE;

	for (ii2 = 0; ii2 < 0x0ffffff; ii2++)
	{
		a2 = step2 * (float)ii2;
		b2 = sinf(a2);
	}

	if (try_wb_write_to_buffer(2)) ok2 = 1;

	for (ii2 = 0; ii2 < 0x0ffffff; ii2++)
	{
		a2 = step2 * (float)ii2;
		b2 = sinf(a2);
	}


	*active = FALSE;
	ExitThread(0);
}

long int ii3;
float a3, b3, step3 = .001f;
DWORD WINAPI my_thread3(LPVOID lp_parameter)
{
	BOOL* active = (BOOL*)lp_parameter;
	*active = TRUE;

	for (ii3 = 0; ii3 < 0x0ffffff; ii3++)
	{
		a3 = step3 * (float)ii3;
		b3 = sinf(a3);
	}

	if (wb_write_to_buffer(3)) ok3 = 1;

	for (ii3 = 0; ii3 < 0x0ffffff; ii3++)
	{
		a3 = step3 * (float)ii3;
		b3 = sinf(a3);
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
	DWORD thread_id3;
	BOOL active1 = TRUE;
	BOOL active2 = TRUE;
	BOOL active3 = TRUE;

	HANDLE h_thread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread1, (void*)&active1, CREATE_SUSPENDED, &thread_id1);
	HANDLE h_thread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread2, (void*)&active2, CREATE_SUSPENDED, &thread_id2);
	HANDLE h_thread3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread3, (void*)&active3, CREATE_SUSPENDED, &thread_id3);

	SetThreadAffinityMask(h_thread1, 1L);// use CPU 0 only
	SetThreadAffinityMask(h_thread2, 2L);// use CPU 1 only
	SetThreadAffinityMask(h_thread3, 3L);// use CPU 2 only
	if (!SetThreadPriority(h_thread1, THREAD_PRIORITY_HIGHEST)) printf("\nSetPriority error\n");
	if (!SetThreadPriority(h_thread2, THREAD_PRIORITY_HIGHEST)) printf("\nSetPriority error\n");
	if (!SetThreadPriority(h_thread3, THREAD_PRIORITY_HIGHEST)) printf("\nSetPriority error\n");

	HANDLE h_process = GetCurrentProcess();
	DWORD dw_process_affinity_mask;
	DWORD dw_system_affinity_mask;
	GetProcessAffinityMask(h_process, &dw_process_affinity_mask, &dw_system_affinity_mask);

	printf("\nThreads IDs %lu %lu %lu\n", thread_id1, thread_id2, thread_id3);

	InitializeCriticalSection(&lps);
	t1 = time(NULL);
	ResumeThread(h_thread1);
	ResumeThread(h_thread2);
	ResumeThread(h_thread3);
	const int thread_priority1 = GetThreadPriority(h_thread1);
	const int thread_priority2 = GetThreadPriority(h_thread2);
	const int thread_priority3 = GetThreadPriority(h_thread3);
	printf("\nHere I am! nmb_prc=%d (thread_priority1 %d thread_priority2 %d thread_priority3 %d): %lx, %lx\n",
		nmb_prc, thread_priority1, thread_priority2, thread_priority3, dw_process_affinity_mask, dw_system_affinity_mask);

	while (active1 == TRUE || active2 == TRUE || active3==TRUE) {}
	DeleteCriticalSection(&lps);
	printf("Wrt2Buf:ok=%d,ok2=%d,ok3=%d \n", ok1, ok2, ok3);
	CloseHandle(h_thread1);
	CloseHandle(h_thread2);
	CloseHandle(h_thread3);
}

int main(int argc, char* argv[])
{
	t1 = time(NULL);
	printf("\nThe number of seconds since January 1, 1970 is %lld", t1);

	init_thread();

	t2 = time(NULL);  t2 = t2 - t1;
	printf("Elapsed time in seconds is %lld,  Bye!\n", t2);
	_getch();
	return 0;
}
