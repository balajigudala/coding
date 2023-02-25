#include<stdio_ext.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

void main()
{
        int fd , ret ;
        char buff[1024],str[100];
        fd = open("/dev/mydevfile",O_RDWR);
        if(fd < 0)
        {
                printf("failed to open\n");
                exit(1);
        }
	while(1)
	{
		printf("enter the string : \n");
		__fpurge(stdin);
		scanf("%[^\n]s",buff);
		if(strcmp(buff,"exit")==0)
			break;
		write(fd ,buff,1024);
	}
	ret=read(fd , str , 1024);
	buff[ret]='\0';
        printf("read string is  %s\n",str);
        close(fd);
}

