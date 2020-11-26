#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

#define SIZE 16

int main()
{
	int ret,fd;
	char buffw[]="hellodriver";

	fd=open("/dev/circbuff",O_RDWR);
	if(fd<0)
	{
		printf("unable to open file\n");
		perror("the error is\n");
		return fd;
	}
	ret = write(fd,buffw,sizeof(buffw));

	printf("wrote %d bytes\n",ret);
	return 0;
}

