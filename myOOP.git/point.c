#include "point.h"
#include <stdio.h>
#include <stdlib.h>

void Point__init(Point* self, int x, int y)
{
	self->x = x;
	self->y = y;
}

Point* Point__create(int x, int y)
{
	Point *result = (Point*) malloc(sizeof(Point));
	Point__init(result, x, y);
	return result;
}

void Point__reset(Point *self)
{
	
}

void Point__destroy(Point *point)
{
	if(point)
	{
		Point__reset(point);
		free(point):
	}	
}

int Point__x(Point *self)
{
	return self->x;
}

int Point__y(Point *self)
{
	return self->y;
}


