
#include "shape.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Shape's prototypes of its virtual functions */
static uint32_t Shape_area_(Shape const * const me);
static void Shape_draw_(Shape const * const me);

//vtbl for Shape class
static struct ShapeVtbl const vtbl = { &Shape_area_, &Shape_draw_};
 
/* constructor */
void Shape_ctor(Shape * const me, char *name , int16_t x, int16_t y) {
     me->vptr = &vtbl; /* "hook" the vptr to the vtbl */
     me->x = x;
     me->y = y;
     me->type = malloc(sizeof(char)*(strlen(name)+1));
     strcpy(me->type, name);
}

/* move-by operation */
void Shape_moveBy(Shape * const me, int16_t dx, int16_t dy) {
    me->x += dx;
    me->y += dy;
}

/* "getter" operations implementation */
int16_t Shape_getX(Shape const * const me) {
    return me->x;
}
int16_t Shape_getY(Shape const * const me) {
    return me->y;
}

char* Shape_getType(Shape const *const me)
{
	return me->type;		
}

/* Shape class implementations of its virtual functions... */
uint32_t Shape_area_(Shape const * const me)
{
    return me->vptr->area(me);
}

void Shape_draw_(Shape const * const me) 
{	
    return me->vptr->draw(me);
}

/* the following code finds the largest-area shape in the collection */
Shape const *largestShape(Shape const *shapes[], uint32_t nShapes) {
    Shape const *s = (Shape *)0;
    uint32_t max = 0U;
    uint32_t i;
    for (i = 0U; i < nShapes; ++i) 
    {
        uint32_t area = Shape_area(shapes[i]); /* virtual call */
	printf("The area is %u \n", area);
        if (area > max) {
            max = area;
            s = shapes[i];
        }
    }
    return s; /* the largest shape in the array shapes[] */
}

/* The following code will draw all Shapes on the screen */
void drawAllShapes(Shape const *shapes[], uint32_t nShapes)
{
    uint32_t i;
    for (i = 0U; i < nShapes; ++i) {
        Shape_draw(shapes[i]); /* virtual call */
    }
}
