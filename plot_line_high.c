#include "header.h"

void plot_line_high(
 int x0,
 int y0,
 int x1,
 int y1,
 int **pxy_arr,
 int *pxy_nbr
)

{

 int *xy_arr;
 int xy_nbr;
 int dx;
 int dy;
 int xi;
 int D;
 int y;
 int x;

 /*
 Initialize xy_arr
 */

 xy_arr= 0;
 xy_nbr= 0;

 dx = x1 - x0;
 dy = y1 - y0;
 xi = 1;
 if ( dx < 0 ) {
    xi = -1;
    dx = -dx;
 }
 D = 2*dx - dy;
 x = x0;

 for ( y= y0 ; y<= y1 ; y++ ) {

    /*
    Add (x,y) to xy_arr
    */

    if ( xy_nbr == 0 ) {
       xy_arr= (int *)calloc((xy_nbr+1),2*sizeof(int));
    }
    else {
       xy_arr= (int *)realloc(xy_arr,(xy_nbr+1)*2*sizeof(int));
    }
    xy_arr[2*xy_nbr+0]= x;
    xy_arr[2*xy_nbr+1]= y;
    xy_nbr++;

    if ( D > 0 ) {
       x = x + xi;
       D = D - 2*dy;
    }
    D = D + 2*dx;
 }

 (*pxy_arr)= xy_arr;
 (*pxy_nbr)= xy_nbr;

}
