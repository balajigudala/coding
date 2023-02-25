/***************************************************************************/
#include<stdio_ext.h>
#include<stdlib.h>
#include<stddef.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
 
#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)
 
int main()
{
        int fd;
        int32_t value, number;
        printf("*********************************\n");
 
        printf("\nOpening Driver\n");
        fd = open("/dev/etx_device", O_RDWR);
        if(fd < 0) 
	{
                printf("Cannot open device file...\n");
                return 0;
        }
	while(1)
	{
		printf("Enter the Value to send \n 1 - write \n 2 - read  \n");
		__fpurge(stdin);
		scanf("%d",&number);
		if(number!=1&&number!=2)
		break;
		switch(number)
		{
			case 1 :
				printf("Writing Value to Driver\n");
				ioctl(fd, WR_VALUE, (int32_t*) &number);
				break;	
			case 2:
				printf("Reading Value from Driver\n");
				ioctl(fd, RD_VALUE, (int32_t*) &number);
				printf("Value is %d\n", value);
				break;
		}
	}
		printf("Closing Driver\n");
		close(fd);
}
