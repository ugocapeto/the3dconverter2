#include "header.h"

void plot_line(
 int x0,
 int y0,
 int x1,
 int y1,
 int **pxy_arr,
 int *pxy_nbr
)

/*
This is the integer implementation of the Bresenham line algorithm
*/

{

 int *xy_arr;
 int xy_nbr;

 if ( abs(y1 - y0) < abs(x1 - x0) ) {
    if ( x0 > x1 ) {
       plot_line_low(
        x1,
        y1,
        x0,
        y0,
        &xy_arr,
        &xy_nbr
       );
    }
    else {
       plot_line_low(
        x0,
        y0,
        x1,
        y1,
        &xy_arr,
        &xy_nbr
       );
    }
 }
 else {
    if ( y0 > y1 ) {
       plot_line_high(
        x1,
        y1,
        x0,
        y0,
        &xy_arr,
        &xy_nbr
       );
    }
    else {
       plot_line_high(
        x0,
        y0,
        x1,
        y1,
        &xy_arr,
        &xy_nbr
       );
    } 
 }

 (*pxy_arr)= xy_arr;
 (*pxy_nbr)= xy_nbr;

}
