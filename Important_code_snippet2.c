#include<stdio.h>
#include<string.h>
#include"Important_code_snippet2.h"

/**************** Extern and static explained *********************/


//int global_var2 = 170; // Multiple defination wont work
//global_var = 70; //Multiple definations of wont work



int a = 5;
int count5()
{
	a++;
	return a;

}


int count6()
{

	b++;
	return b;

}

int count7()
{
	c = 20;
	c++;
	return c;

}

int count8()
{
	int d = 20;
	d++;
	return d;
}

int main()
{

	int num = 4;
	while(num--)
	{
		printf("%d\t", count5());

	}
	printf("\n");

	num = 4;
	while(num--)
	{
		printf("%d\t", count6());

	}
	printf("\n");

	num = 4;
	while(num--)
	{
		printf("%d\t", count7());

	}
	printf("\n");

	num = 4;
	while(num--)
	{
		printf("%d\t", count8());

	}
	printf("\n");

	printf("Extern variable is %d\n", ++global_var);

	int global_var1 = 200;
	printf("%d\n", global_var1);
	printf("%d\n", global_var2);
	return 0;
}
