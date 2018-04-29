#ifndef _SHAPE_H_
#define _SHAPE_H_

#include<stdio.h>
#include<stdlib.h>

typedef struct shape
{
	int x;
	int y;
}shape;

void shape_ctor(shape *self, const int x, const int y);
int shape_getX(shape *self);
int shape_getY(shape *self);
void shape_moveBy(shape *self, int dx, int dy); 
#endif
