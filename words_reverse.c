#include<stdio.h>
#include<string.h>
#include<stdio_ext.h>
void main()
{
	char arr[100];
	int i=0,j=0,k=0,temp=0,l=0;
	printf("enter the string\n");
	scanf("%[^\n]s",arr);
	__fpurge(stdin);
	l=strlen(arr);
	for(i=0;arr[i]!='\0';i++)
	{
		if((arr[i]==' ')||(i==l))
		{
			j=i-1;
			for(k,j;k<j;k++,j--)
			{
				temp=arr[k];
				arr[k]=arr[j];
				arr[j]=temp;
			}
			k=i+1;
		}
	}
	printf("%s",arr);
}

