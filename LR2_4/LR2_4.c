#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#define THREAD_COUNT 4

HANDLE gh_write_event;
HANDLE gh_threads[THREAD_COUNT];

DWORD WINAPI thread_proc(LPVOID);

void create_events_and_threads(void)
{
	DWORD dw_thread_id;

	gh_write_event = CreateEvent(NULL, TRUE, FALSE, TEXT("WriteEvent"));

	if (gh_write_event == NULL)
	{
		printf("CreateEvent failed (%lu)\n", GetLastError());
		return;
	}

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		gh_threads[i] = CreateThread(NULL, 0, thread_proc, NULL, 0, &dw_thread_id);

		if (gh_threads[i] == NULL)
		{
			printf("CreateThread failed (%lu)\n", GetLastError());
			return;
		}
	}
}

void write_to_buffer(VOID)
{
	if (!SetEvent(gh_write_event))
	{
		printf("SetEvent failed (%lu)\n", GetLastError());
	}
}

void close_events()
{
	CloseHandle(gh_write_event);
}

time_t t, t1;

int main(void)
{
	create_events_and_threads();
	printf("Main thread will write to the shared buffer if you press any key...\n");
	_getch();
	t = time(NULL);

	write_to_buffer();

	printf("Main thread waiting for threads to exit...\n");

	const DWORD dw_wait_result = WaitForMultipleObjects(THREAD_COUNT, gh_threads, TRUE, INFINITE);


	if (dw_wait_result == WAIT_OBJECT_0)
	{
		printf("All threads ended, cleaning up for application exit...\n");
		t1 = time(NULL);  t1 = t1 - t;
		printf("Elapsed time in seconds is %lld,  Bye!\n", t1);
	}
	else
	{
		printf("WaitForMultipleObjects failed (%lu)\n", GetLastError());
	}

	close_events();
	printf("Main function terminated, press any key!\n"); _getch();
	return 0;
}

DWORD WINAPI thread_proc(LPVOID lp_param)
{
	UNREFERENCED_PARAMETER(lp_param);

	printf("Thread %lu waiting for write event...\n", GetCurrentThreadId());

	const DWORD dw_wait_result = WaitForSingleObject(gh_write_event, INFINITE);

	if (dw_wait_result == WAIT_OBJECT_0)
	{
		printf("Thread %lu reading from buffer\n", GetCurrentThreadId());
		Sleep(3000L);
	}
	else
	{
		printf("Wait error (%lu)\n", GetLastError());
		return 0;
	}

	printf("Thread %lu exiting\n", GetCurrentThreadId());
	return 1;
}



