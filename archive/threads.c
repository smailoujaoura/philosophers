#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int x = 42;
void *thread_task(void *arg)
{
    ++x;
    printf("Thread: Value of x = %d\n", x);
    return NULL;
}
int main()
{
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread_task, NULL);
    pthread_create(&thread2, NULL, thread_task, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}