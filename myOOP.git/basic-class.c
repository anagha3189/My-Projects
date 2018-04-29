#include<string.h>
#include "basic-class.h"

void shape_ctor(shape *self, int x, int y)
{
	self->x = x;
	self->y = y;
}

int  shape_getX(shape *self)
{
	return self->x;
}

int shape_getY(shape *self)
{
	return self->y;
}

void shape_moveBy(shape *self, int dx, int dy)
{
	self->x += dx;
	self->y += dy;
} 


/*int main()
{
	shape s1, s2;
	shape_ctor(&s1, 3, 5);
	printf("shape s1 : x = %d, y = %d\n", shape_getX(&s1), shape_getY(&s1));
	shape_moveBy(&s1, -1, -1);
	printf("shape s1 : x = %d, y = %d\n", shape_getX(&s1), shape_getY(&s1)); 
	return 0;
}*/
