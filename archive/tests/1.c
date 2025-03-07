#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void* thread_func(void* arg) 
{
    printf("Hello from the thread!\n");
    sleep(5);
    (void)arg;
    return NULL;
}

int main() 
{
    pthread_t thread_id;

    if (pthread_create(&thread_id, NULL, thread_func, NULL) != 0) 
    {
        perror("Failed to create thread");
        return 1;
    }
    sleep(2);
    // pthread_join(thread_id, NULL);
    printf("Exiting main function.\n");
    return 0;
}