#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#define SIZE 16

int main()
{
	int ret, fd;
	char buffw[]="FromUser";
	char buffr[SIZE];
	fd = open("/dev/circ_buff",O_RDWR);
	if(fd<0)
	{
		printf("file is not opening\n");
		perror("not opened\n");
		return fd;
	}
	ret = write(fd,buffw,sizeof(buffw));

	ret = read(fd,buffr,SIZE);

	return 0;  //close system call will be called implicitly by os
}
