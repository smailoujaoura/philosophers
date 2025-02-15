#include <stdio.h>     // Required for printf
#include <sys/time.h>  // Required for gettimeofday (time tracking)
#include <unistd.h>    // Required for usleep

// The timeval structure provides a representation for storing time:
// tv_sec stores the number of seconds while tv_usec stores the number of microseconds.
// struct timeval {
//     time_t      tv_sec;     // seconds
//     suseconds_t tv_usec;    // microseconds
// };

void usleep_glitch()
{
    // Declare two timeval structures to store the start and end timestamps.
    struct timeval  start_time;
    struct timeval  end_time;

    // Store the desired sleep time, in this case, 200 milliseconds or 200,000 microseconds.
    long requested_sleep_time = 200 * 1000;

    // Capture the current time and store it in start_time. This marks the beginning of our usleep.
    gettimeofday(&start_time, NULL);

    // usleep is used to pause the execution of the program.
    // Here, the intention is to pause for 200,000 microseconds or 200 milliseconds.
    usleep(requested_sleep_time);

    // Once usleep completes, immediately capture the current time and store it in end_time.
    gettimeofday(&end_time, NULL);

    // Calculate the actual time the program was asleep.
    // This is done by finding the difference between the start and end times.
    long actual_sleep_time = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);

    // Print the desired sleep time and the actual sleep time.
    printf("Requested Sleep Time: \t\t%ld microseconds\n", requested_sleep_time);
    printf("Actual Sleep Time: \t\t%ld microseconds\n\n", actual_sleep_time);
}

int main()
{
    // Call the example function to demonstrate the potential inconsistency in usleep.
    usleep_glitch();

    return (0);
}