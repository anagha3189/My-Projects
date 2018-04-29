#include<string.h>
#include "dynamic-basic-class.h"

person* new_person(const char *fN , const char *lN)
{
	person *self = (person*)malloc(sizeof(person));
	if(self == NULL)
	return NULL;
	self->fName = malloc(sizeof(char) * (strlen(fN)+1));
	strcpy(self->fName , fN);
	self->lName = malloc(sizeof(char) * (strlen(lN)+1));
	strcpy(self->lName , lN);
	self->Display = person_display;
	self->Delete = delete_person; 
	return self;
}

void person_display(person *self)
{
	printf("First Name: %s , Last Name : %s\n", self->fName, self->lName);
}

void delete_person(person *self)
{
	if(self)
	{
		free(self->fName);
		free(self->lName);
	}
	free(self);
}

int main()
{
	person *p = new_person("Anagha", "Kate");
	p->Display(p);
	p->Delete(p);
	p = NULL;
	return 0;
}







