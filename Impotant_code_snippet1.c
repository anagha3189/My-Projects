

//This is to understand the static, local, and dynamic variable	
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


static int b = 10;

char fun()
{
   char str[10];
   strcpy(str,"Hello!");
   return(*str); //Will return 'H' as temp char will get created while passing
}

char* fun1()
{
   char str[10];
   strcpy(str,"Hello!");
   return(str); //Cannot return the address of the local variable
}

int fun2()
{
    int a = 5;
    return a; //Will return 5 as temp char will get created while passing
}


int* fun3()
{
    int a = 5;
    return &a; //Cannot return address of the local variable
}

char *fun4()
{
    char *str = (char*)malloc(15 * sizeof(char));
    strcpy(str, "I am Anagha");
    return str; 
}

int count()
{
    static int a = 10;
    a++;
    return a;
}

int count1()
{
    int b = 5;
    b++;
    return b;
}


int main()
{
    char a = fun();
    printf("%c\n" , a);
    //char* b = fun1();
    //printf("%s\n" , b);
    int c = fun2();
    printf("%d\n" , c);
    //int *d = fun3();
    //printf("%a\n" , d);
    char *e = fun4();
    printf("%s\n", e);
    int num = 4;
    while(num--)
    {
        printf("%d\n", count());
    }
    
    num = 4;
    while(num--)
    {
        printf("%d\n", count1());
    }
    
    return 0;
}
