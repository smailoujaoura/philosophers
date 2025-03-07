#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    write(1, "test\n", 6);
    int fd = open("Makefile", O_RDONLY);
    int fd2 = open("parsing.c", O_RDONLY);
    struct timeval tv;
    int i = 0;
    for (; i<100; i++) {
            gettimeofday(&tv,NULL);
    }
    close(fd);
    return 0;
}