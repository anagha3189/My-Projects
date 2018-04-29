#ifndef PERSON_H
#define PERSON_H

#include<stdio.h>
#include<stdlib.h>

typedef struct _person person;
typedef void (*fptrDisplay)(person *);
typedef void (*fptrDelete)(person *);

typedef struct _person
{
	char *fName;
	char *lName;
	fptrDisplay Display;
	fptrDelete Delete; 
}person;

person* new_person(const char *fName , const char *lName);
void person_display(person *self);
void delete_person(person *self);

#endif


