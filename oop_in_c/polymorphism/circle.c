
#include "circle.h"  /* Circle class interface */
#include <stdio.h> /* for printf() */

/* NOTE: the "me" pointer has the type of the superclass to fit the vtable */
static uint32_t Circle_area_(Shape const * const me);
static void Circle_draw_(Shape const * const me);

/* constructor */
void Circle_ctor(Circle * const me, int16_t x, int16_t y,
                 uint16_t rad, char *name)
{
    static struct ShapeVtbl const vtbl = { /* vtbl of the Circle class */
        &Circle_area_,
        &Circle_draw_
    };
    Shape_ctor(&me->super, name, x, y); /* call the superclass' ctor */
    me->super.vptr = &vtbl; /* override the vptr */
    me->rad = rad;
}

/* Circle's class implementations of its virtual functions... */
static uint32_t Circle_area_(Shape const * const me) {
    Circle const * const me_ = (Circle const *)me; /* explicit downcast */
    /* pi is approximated as 3 */
    return 3U * (uint32_t)me_->rad * (uint32_t)me_->rad;
}

static void Circle_draw_(Shape const * const me) {
    Circle const * const me_ = (Circle const *)me; /* explicit downcast */
    printf("Circle_draw_(rad=%d)\n", me_->rad);
}

