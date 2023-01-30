//only applicable for replaceing of larger strings
#include<stdio_ext.h>
#include<string.h>
#include<stdlib.h>
int m,n,o,diff,cnt=0,cnt1=0;
char * read(char *);
void replace(char *,char*,char*);
int len1,len2,len3,diff1,diff2;
int main()
{
        char *ptr1,*ptr2,*ptr3;
	int i,j,k;
	printf("enter the main string : ");
	ptr1=read(ptr1);
	printf("enter the search string : ");
	ptr2=read(ptr2);
	printf("enter the replace string : ");
	ptr3=read(ptr3);
//	printf("%s \n%s \n%s \n",ptr1,ptr2,ptr3);
	replace(ptr1,ptr2,ptr3);
}

char * read(char *ptr)
{
	int len,j;
	char *buff=(char*)calloc(500,sizeof(char));
	__fpurge(stdin);
	scanf("%[^\n]",buff);
	len=strlen(buff);
	buff=(char*)realloc(buff,(len)*sizeof(char));
	return buff;
}

void replace(char *ptr1,char*ptr2,char*ptr3)
{
	int i,j,k,l;
	cnt=0,cnt1=0;
	m=strlen(ptr1);
	n=strlen(ptr2);
        o=strlen(ptr3);
	diff=o-n;
	for(i=0,j=0;ptr1[i]!='\0';i++)
	{
		if(ptr1[i]==ptr2[j])
		{
			j++;
			cnt++;
			if(cnt==n)
			{
				//m=(strlen(ptr1)+diff);
				
				//ptr1=(char*)realloc(ptr1,((m=strlen(ptr1))+diff)*sizeof(char));
				if(diff > 0)
					ptr1=(char*)realloc(ptr1,((m=strlen(ptr1))+diff)*sizeof(char));
				//printf("realloc count =%d\n",m);
				for(i=i-(n-1),k=0;ptr3[k]!='\0';i++,k++)
				{
		//			printf("i:%d\n",i);
					if(n>o)
					{    for(l=i+(n-1);l<=m;l++)
                                                {
                                                        ptr1[l]=ptr1[l+1];
                                                }
                                                
					}
					else if(k>(n-1))
					{
						for(l=m;l>=i;l--)
						{
							ptr1[l+1]=ptr1[l];
						}
						m++;
					}
				//	else
						ptr1[i]=ptr3[k];
				}
				cnt1++;
				cnt=0;
				j=0;
				if(n>o)
					i = i + diff;
				else 
					i--;
			}
			continue;
		}
		cnt=0;
		j=0;
	}
//	printf("count=%d\n",cnt1);
	printf("string : %s\n",ptr1);
}

