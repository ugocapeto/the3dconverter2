#include "header.h"

void plot_line_low(
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
 int yi;
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
 yi = 1;
 if ( dy < 0 ) {
    yi = -1;
    dy = -dy;
 }
 D = 2*dy - dx;
 y = y0;

 for ( x= x0 ; x<= x1 ; x++ ) {

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
       y = y + yi;
       D = D - 2*dx;
    }
    D = D + 2*dy;
 }

 (*pxy_arr)= xy_arr;
 (*pxy_nbr)= xy_nbr;

}
