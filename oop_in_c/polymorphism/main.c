

#include "rect.h"   /* Rectangle class interface */
#include "circle.h" /* Circle class interface */
#include <stdio.h>  /* for printf() */

int main() {
    Rectangle r1, r2; /* multiple instances of Rectangle */
    Circle    c1, c2; /* multiple instances of Circle */

    /* instantiate rectangles... */
    Rectangle_ctor(&r1, 0, 2, 10, 15,"rectangle");
    Rectangle_ctor(&r2, -1, 3, 5, 8,"rectangle" );

    /* instantiate circles... */
    Circle_ctor(&c1, 1, -2, 12,"circle");
    Circle_ctor(&c2, 1, -3, 6, "circle");

    Shape *ptr = (Shape*)&c1;
    Shape *ptr1 = (Shape*)&r1;
    Shape_draw(ptr);
    printf("The area is %u \n", Shape_area(ptr)); 
    Shape_draw(ptr1);
    printf("The area is %u  \n", Shape_area(ptr1));
  
    return 0;
}



