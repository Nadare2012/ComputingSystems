#include <windows.h>
#include <stdio.h>
HANDLE h_sem;
int a[5];
HANDLE h_thread;
unsigned long thread_id;

DWORD WINAPI thread()
{
    int num = 0;
    while (1)
    {
        WaitForSingleObject(h_sem, INFINITE);
        for (int i = 0; i < 5; i++) a[i] = num;
        ReleaseSemaphore(h_sem, 1, NULL);
        num++;
    }
}

int main(void)
{
    h_sem = CreateSemaphore(NULL, 1, 1, NULL);
    h_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread, NULL, 0, &thread_id);
    while (1)
    {
        WaitForSingleObject(h_sem, INFINITE);
        printf("%d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4]);
        ReleaseSemaphore(h_sem, 1, NULL);
    }
}