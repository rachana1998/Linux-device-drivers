#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include"ioctlheader.h"

#define SIZE 16

int main()
{
        int fd, ret;

        char buffw[]= "hellodriver";

        fd = open("/dev/sample0",O_RDWR);
        if(fd<0)
        {
                printf("unable to open file\n");
                perror("the error is\n");
                return fd;
        }
	ret = ioctl(fd,CMD_1);

        ret =write(fd,buffw,sizeof(buffw));
        printf("the no.of bytes wrote is %d\n",ret);
        return 0;
}

