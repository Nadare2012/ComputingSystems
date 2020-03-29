#pragma hdrstop
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <stdint.h>
#include <time.h>
#include <windows.h>

time_t t1, t2;

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
	computing_func(INT32_MAX / 2);
	*active = FALSE;
	ExitThread(0);
}

DWORD WINAPI my_thread_func2(LPVOID lp_parameter)
{
	BOOL* active = (BOOL*)lp_parameter;
	*active = TRUE;
	computing_func(INT32_MAX / 2);
	*active = FALSE;
	ExitThread(0);
}

void init_thread(void)
{
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	int nmb_prc = system_info.dwNumberOfProcessors;
	printf("\nNumber of CPU Cores: nmbPrc=%d\n", nmb_prc);

	DWORD thread_id1;
	DWORD thread_id2;
	BOOL active1 = TRUE;
	BOOL active2 = TRUE;

	HANDLE h_thread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_func1, (void*)&active1, 0, &thread_id1);
	HANDLE h_thread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_func2, (void*)&active2, 0, &thread_id2);

	printf("\nThreads IDs %lu %lu \n", thread_id1, thread_id2);

	t1 = time(NULL);

	HANDLE h_process = GetCurrentProcess();
	DWORD dw_process_affinity_mask;
	DWORD dw_system_affinity_mask;
	GetProcessAffinityMask(h_process, &dw_process_affinity_mask, &dw_system_affinity_mask);

	printf("\nProcessAffinityMask / SystemAffinityMask: %lx / %lx\n", dw_process_affinity_mask, dw_system_affinity_mask);

	while (active1 == TRUE || active2 == TRUE) {}

	CloseHandle(h_thread1);
	CloseHandle(h_thread2);
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
