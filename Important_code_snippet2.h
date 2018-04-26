#include<stdio.h>
#include<string.h>


extern int a; // This will act as a extern global 
static int b = 10; // Static will act as static 
static int c; //static becomes local if not initialized in .h
static int d = 50; // Local gets preferred over static
extern int global_var;//Understand the use of extern. Check in extern_file.c
extern int global_var1;
extern int global_var2;
