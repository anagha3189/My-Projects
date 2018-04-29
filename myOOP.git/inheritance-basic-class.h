#ifndef _RECT_H_
#define _RECT_H_

#include<stdio.h>
#include<stdlib.h>
#include "basic-class.h"

typedef struct rect
{
	shape super;
	int width;
	int height;	
}rect;

void rect_ctor(rect *self, int x, int y, int width, int height);
int area(rect *self);

#endif
