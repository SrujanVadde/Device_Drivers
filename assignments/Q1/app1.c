#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
        int fd;
        char option;
        printf("Welcome to device Drivers\n");
        fd = open("/dev/driver2", O_RDWR);
        if(fd < 0)
{
                printf("Cannot open device file...\n");
                return 0;
}


close(fd);
return 0;
}
