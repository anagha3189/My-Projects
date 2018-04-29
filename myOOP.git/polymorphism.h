#ifndef _POLYMORPHISM_H_
#define _POLYMORPHISM_H_

typedef struct polygon
{
	int width;
	int height;
	void (display*)();
	int (area*) (int, int);
}polygon;

typedef struct rectangle
{
	polygon *p;
	char *type;
}rectangle;

typedef struct triangle
{
	polygon *p;
	char *type;
}triangle;

rectangle * create(int width, int height, 
int rect_area(int width, int height);
int tri_area(int width , int height);
void rect_display();
void tri_display();


#endif






