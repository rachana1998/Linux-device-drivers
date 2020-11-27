#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

#define SIZE 16

int main()
{
        int fd, ret;

        char buffw[]= "hellodriver";

        fd = open("/dev/sample2",O_RDWR);
        if(fd<0)
        {
                printf("unable to open file\n");
                perror("the error is\n");
                return fd;
        }
        ret =write(fd,buffw,sizeof(buffw));
        printf("the no.of bytes wrote is %d\n",ret);
        return 0;
}

