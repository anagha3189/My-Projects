#ifndef C_POINT_H
#define C_POINT_H

struct Point
{
	int x;
	int y;
}; 

Point* Point__create(int x, int y):
void Point__destroy(Point *self);
int Point__x(Point* self);
int Point__y(Point* self);

#endif
