#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

int main()
{

	int fd;
	char *str;
	fd = open("/dev/dummy_driver",O_RDONLY);
	if ( fd > 0)
	{
		printf("open successful\n");
		str = mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0);
		if(str == MAP_FAILED)
		{
			printf("errno %d\n",errno);
			perror("mmap failed: ");
		}	
		printf("%s \n",str);
	}
	else
		printf("open failed");
	close(fd);
	return 0;
}