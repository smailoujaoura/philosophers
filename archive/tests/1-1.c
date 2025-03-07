#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>

int main(void)
{
    const char *text = "What is what is yess?\n";
    int len = strlen(text);
    
    int fd = open("my_tempo_file", O_RDWR | O_CREAT, 0644);
	flock(fd, LOCK_EX);
    write(fd, text, len);
    close(fd);
    
    sleep(5);

    fd = open("my_tempo_file", O_RDONLY);
	if (fd == -1)
	{
		perror("some thing wrong: ");
		return (0);
	}
    char buf[24];
    read(fd, buf, len);
    printf("[%s]", buf);
    close(fd);
    
    return 0;
}