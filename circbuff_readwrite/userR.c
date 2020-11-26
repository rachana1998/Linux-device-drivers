#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

#define SIZE 16

int main()
{
	int fd, ret;

	char buffr[SIZE];

	fd = open("/dev/circbuff",O_RDWR);
	if(fd<0)
	{
		printf("unable to open file\n");
		perror("the error is\n");
		return fd;
	}
	ret = read(fd,buffr,SIZE);
	printf("the read data is %s\n",buffr);
	printf("the no.of bytes read is %d\n",ret);
	return 0;
}
