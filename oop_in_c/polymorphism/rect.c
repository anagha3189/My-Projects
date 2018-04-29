

#include "rect.h"  /* Rectangle class interface */
#include <stdio.h> /* for printf() */

/* Rectangle's prototypes of its virtual functions */
/* NOTE: the "me" pointer has the type of the superclass to fit the vtable */
static uint32_t Rectangle_area_(Shape const * const me);
static void Rectangle_draw_(Shape const * const me);

//vtabl for Rectangle class
static struct ShapeVtbl const vtbl = { &Rectangle_area_, &Rectangle_draw_ };
   
/* constructor */
void Rectangle_ctor(Rectangle * const me, int16_t x, int16_t y, uint16_t width, 
			uint16_t height, char *name)
{
    Shape_ctor(&me->super, name, x, y); /* call the superclass' ctor */
    me->super.vptr = &vtbl; /* override the vptr */
    me->width = width;
    me->height = height;
}

/* Rectangle's class implementations of its virtual functions... */
static uint32_t Rectangle_area_(Shape const * const me) 
{
    Rectangle const * const me_ = (Rectangle const *)me; /* explicit downcast */
    return (uint32_t)me_->width * (uint32_t)me_->height;
}

static void Rectangle_draw_(Shape const * const me) 
{
    Rectangle const * const me_ = (Rectangle const *)me; /* explicit downcast */
    printf("Draw: %s with width=%d,height=%d \n", Shape_getType(me), me_->width, me_->height);
}


