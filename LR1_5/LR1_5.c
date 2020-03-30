#pragma hdrstop
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <stdint.h>
#include <time.h>
#include <windows.h>

time_t t1, t2;
HANDLE mutex;

void computing_func(const int loop_length)
{
	double tmp = INT32_MAX;
	for (int i = 0; i < loop_length; ++i)
		tmp /= (atan(1.0) * 4);
	for (int i = 0; i < loop_length; ++i)
		tmp *= (atan(1.0) * 4);
}

DWORD WINAPI my_thread_func1(LPVOID lp_parameter)
{
	BOOL* active = (BOOL*)lp_parameter;
	*active = TRUE;
	WaitForSingleObject(mutex, INFINITE);
	computing_func(INT32_MAX / 2);
	ReleaseMutex(mutex);
	*active = FALSE;
	ExitThread(0);
}

DWORD WINAPI my_thread_func2(LPVOID lp_parameter)
{
	BOOL* active = (BOOL*)lp_parameter;
	*active = TRUE;
	WaitForSingleObject(mutex, INFINITE);
	computing_func(INT32_MAX / 16);
	ReleaseMutex(mutex);
	*active = FALSE;
	ExitThread(0);
}

DWORD WINAPI my_thread_func3(LPVOID lp_parameter)
{
	BOOL* active = (BOOL*)lp_parameter;
	*active = TRUE;
	WaitForSingleObject(mutex, INFINITE);
	computing_func(INT32_MAX / 16);
	ReleaseMutex(mutex);
	*active = FALSE;
	ExitThread(0);
}

DWORD WINAPI my_thread_func4(LPVOID lp_parameter)
{
	BOOL* active = (BOOL*)lp_parameter;
	*active = TRUE;
	WaitForSingleObject(mutex, INFINITE);
	computing_func(INT32_MAX / 16);
	ReleaseMutex(mutex);
	*active = FALSE;
	ExitThread(0);
}

DWORD WINAPI my_thread_func5(LPVOID lp_parameter)
{
	BOOL* active = (BOOL*)lp_parameter;
	*active = TRUE;
	WaitForSingleObject(mutex, INFINITE);
	computing_func(INT32_MAX / 16);
	ReleaseMutex(mutex);
	*active = FALSE;
	ExitThread(0);
}

DWORD WINAPI my_thread_func6(LPVOID lp_parameter)
{
	BOOL* active = (BOOL*)lp_parameter;
	*active = TRUE;
	WaitForSingleObject(mutex, INFINITE);
	computing_func(INT32_MAX / 16);
	ReleaseMutex(mutex);
	*active = FALSE;
	ExitThread(0);
}

DWORD WINAPI my_thread_func7(LPVOID lp_parameter)
{
	BOOL* active = (BOOL*)lp_parameter;
	*active = TRUE;
	WaitForSingleObject(mutex, INFINITE);
	computing_func(INT32_MAX / 16);
	ReleaseMutex(mutex);
	*active = FALSE;
	ExitThread(0);
}

DWORD WINAPI my_thread_func8(LPVOID lp_parameter)
{
	BOOL* active = (BOOL*)lp_parameter;
	*active = TRUE;
	WaitForSingleObject(mutex, INFINITE);
	computing_func(INT32_MAX / 16);
	ReleaseMutex(mutex);
	*active = FALSE;
	ExitThread(0);
}

void init_thread(void)
{
	mutex = CreateMutex(NULL, FALSE, NULL);

	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	int nmb_prc = system_info.dwNumberOfProcessors;
	printf("\nNumber of CPU Cores: nmbPrc=%d\n", nmb_prc);

	DWORD thread_id1;
	DWORD thread_id2;
	DWORD thread_id3;
	DWORD thread_id4;
	DWORD thread_id5;
	DWORD thread_id6;
	DWORD thread_id7;
	DWORD thread_id8;
	BOOL active1 = TRUE;
	BOOL active2 = TRUE;
	BOOL active3 = TRUE;
	BOOL active4 = TRUE;
	BOOL active5 = TRUE;
	BOOL active6 = TRUE;
	BOOL active7 = TRUE;
	BOOL active8 = TRUE;

	HANDLE h_thread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_func1, (void*)&active1, 0, &thread_id1);
	HANDLE h_thread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_func2, (void*)&active2, 0, &thread_id2);
	HANDLE h_thread3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_func3, (void*)&active3, 0, &thread_id3);
	HANDLE h_thread4 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_func4, (void*)&active4, 0, &thread_id4);
	HANDLE h_thread5 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_func5, (void*)&active5, 0, &thread_id5);
	HANDLE h_thread6 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_func6, (void*)&active6, 0, &thread_id6);
	HANDLE h_thread7 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_func7, (void*)&active7, 0, &thread_id7);
	HANDLE h_thread8 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_func8, (void*)&active8, 0, &thread_id8);

	SetThreadAffinityMask(h_thread1, 1 << 0);
	SetThreadAffinityMask(h_thread2, 1 << 1);
	SetThreadAffinityMask(h_thread3, 1 << 2);
	SetThreadAffinityMask(h_thread4, 1 << 3);
	SetThreadAffinityMask(h_thread5, 1 << 4);
	SetThreadAffinityMask(h_thread6, 1 << 5);
	SetThreadAffinityMask(h_thread7, 1 << 6);
	SetThreadAffinityMask(h_thread8, 1 << 7);

	SetThreadPriority(h_thread1, THREAD_PRIORITY_HIGHEST);

	printf("\nThreads IDs %lu %lu %lu %lu %lu %lu %lu %lu\n", thread_id1, thread_id2, thread_id3, thread_id4, thread_id5, thread_id6, thread_id7, thread_id8);

	t1 = time(NULL);

	HANDLE h_process = GetCurrentProcess();
	DWORD dw_process_affinity_mask;
	DWORD dw_system_affinity_mask;
	GetProcessAffinityMask(h_process, &dw_process_affinity_mask, &dw_system_affinity_mask);

	printf("\nProcessAffinityMask / SystemAffinityMask: %lx / %lx\n", dw_process_affinity_mask, dw_system_affinity_mask);

	while (active1 == TRUE || active2 == TRUE || active3 == TRUE || active4 == TRUE || active5 == TRUE || active6 == TRUE || active7 == TRUE || active8 == TRUE) {}

	CloseHandle(h_thread1);
	CloseHandle(h_thread2);
	CloseHandle(h_thread3);
	CloseHandle(h_thread4);
	CloseHandle(h_thread5);
	CloseHandle(h_thread6);
	CloseHandle(h_thread7);
	CloseHandle(h_thread8);
	CloseHandle(mutex);
}

int main(int argc, char* argv[])
{
	t1 = time(NULL);
	printf("\nThe number of seconds since January 1, 1970 is %lld", t1);

	init_thread();
	t2 = time(NULL);
	t2 = t2 - t1;
	printf("Elapsed time in seconds is %lld, Bye!\n", t2);
	_getch();
	return 0;
}
