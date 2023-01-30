#include<stdio.h>
int ROW;
int COL;
void read_input(int(*ptr)[COL]);
void display_output(int(*ptr)[COL]);
int main()
{
	printf("enter the rows and coloumns of the matrix : \n");
	scanf("%d%d",&ROW,&COL);
	int ptr[ROW][COL];
	//printf("\nenter the elements of the matrix :\n");
	read_input(ptr);
	printf("\nthe spiral matrix of given matrix is:\n");
	display_output(ptr);
	printf("\n");
}
void read_input(int(*ptr)[COL])
{
	int i,j,l=0;
	for(i=0;i<ROW;i++)
	{
		for(j=0;j<COL;j++)
		{
		//	scanf("%d",&ptr[i][j]);
		        ptr[i][j]=++l;
		}
		printf("\n");
	}
}
void display_output(int(*ptr)[COL])
{
	int i=-1,j=0,k,temp;
	int elements=ROW*COL;
	temp=COL-1;
	int n=0;
	//printf("TEMp value%d\n",temp);
	while(elements>0)
	{
		i++;
		for(j=i;j<=temp;j++)
		{
			if(elements==0)
				break;
			printf("%d ",ptr[i][j]);
			elements--;
			
		}
		for(k=i+1,j--;k<=temp;k++)
		{
			if(elements==0)
				break;
			printf("%d ",ptr[k][j]);
			elements--;
		}
		for(j--,k--;j>=n;j--)
		{
			if(elements==0)
				break;
			printf("%d ",ptr[k][j]);
			elements--;
		}
		for(k--,j++;k>n;k--)
		{
			if(elements==0)
				break;
			printf("%d ",ptr[k][j]);
			elements--;
		}
		temp--;
		n++;
	}
}
