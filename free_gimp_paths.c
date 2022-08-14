#include "header.h"

void free_gimp_paths(
 int gimp_path_nbr,
 gimp_path_struct *gimp_path_arr
)

{

 int gimp_path_ind;
 int gimp_sub_path_ind;
 int gimp_sub_path_nbr;
 int gimp_sub_path_bezier_curve_nbr;
 gimp_sub_path_struct *gimp_sub_path_arr;
 gimp_sub_path_bezier_curve_struct *gimp_sub_path_bezier_curve_arr;
 gimp_path_struct gimp_path_data;
 gimp_sub_path_struct gimp_sub_path_data;

 for ( gimp_path_ind= 0 ;
       gimp_path_ind< gimp_path_nbr ;
       gimp_path_ind++ ) {
    gimp_path_data= gimp_path_arr[gimp_path_ind];
    gimp_sub_path_nbr= gimp_path_data.gimp_sub_path_nbr;
    gimp_sub_path_arr= gimp_path_data.gimp_sub_path_arr;

    for ( gimp_sub_path_ind= 0 ;
          gimp_sub_path_ind< gimp_sub_path_nbr ;
          gimp_sub_path_ind++ ) {
       gimp_sub_path_data= gimp_sub_path_arr[gimp_sub_path_ind];
       gimp_sub_path_bezier_curve_nbr= gimp_sub_path_data.gimp_sub_path_bezier_curve_nbr;
       gimp_sub_path_bezier_curve_arr= gimp_sub_path_data.gimp_sub_path_bezier_curve_arr;

       if ( gimp_sub_path_bezier_curve_nbr > 0 )
        free(gimp_sub_path_bezier_curve_arr);
    }

    if ( gimp_sub_path_nbr > 0 )
     free(gimp_sub_path_arr);
 }

 if ( gimp_path_nbr > 0 )
  free(gimp_path_arr);

}
