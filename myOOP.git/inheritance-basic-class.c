#include "inheritance-basic-class.h"

void rect_ctor(rect *self, int x, int y, int width, int height)
{
	shape_ctor(&self->super, x, y);
	self->width = width;
	self->height = height;
}

int area(rect *self)
{
	return self->width * self->height;
}

int main()
{
	rect r1;
	rect_ctor(&r1, 3, 5, 4, 6);
	printf("r1.super.x = %d, r1.super.y = %d\n",shape_getX(&(r1.super)), shape_getY(&(r1.super)));
	printf("rect area is = %d\n", area(&r1));
	return 0;
}
