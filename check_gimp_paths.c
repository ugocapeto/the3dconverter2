#include "header.h"

void check_gimp_paths(
 int gimp_path_nbr,
 gimp_path_struct *gimp_path_arr
)

{

 int gimp_path_ind;
 int gimp_sub_path_ind;
 int gimp_sub_path_bezier_curve_ind;
 int gimp_sub_path_nbr;
 int gimp_sub_path_bezier_curve_nbr;
 gimp_sub_path_struct *gimp_sub_path_arr;
 gimp_sub_path_bezier_curve_struct *gimp_sub_path_bezier_curve_arr;
 gimp_path_struct gimp_path_data;
 gimp_sub_path_struct gimp_sub_path_data;
 gimp_sub_path_bezier_curve_struct gimp_sub_path_bezier_curve_data;
 int control_point_ind;
 double control_point_arr[4][2];
 int k;
 char gimp_path_id[MAXLINE];
 char variable_string[MAXLINE];
 int variable_int;

 for ( gimp_path_ind= 0 ;
       gimp_path_ind< gimp_path_nbr ;
       gimp_path_ind++ ) {
    gimp_path_data= gimp_path_arr[gimp_path_ind];
    gimp_sub_path_nbr= gimp_path_data.gimp_sub_path_nbr;
    gimp_sub_path_arr= gimp_path_data.gimp_sub_path_arr;
    strcpy(gimp_path_id,gimp_path_arr[gimp_path_ind].gimp_path_id);

    /*
    The gimp path id must start with a 3 character string,
    which when converted to an integer should range from 0 to 255
    */

    strncpy(variable_string,gimp_path_id,3);
    variable_int= atoi(variable_string);
    if ( !(variable_int>=0 && variable_int<=255) ) {
       fprintf(stdout,"Format for gimp path id %s is not valid!\n",gimp_path_id);
       error_handler((char *)"check_gimp_paths");
    }

    /*
    Let's go through the gimp path components aka sub paths
    */

    for ( gimp_sub_path_ind= 0 ;
          gimp_sub_path_ind< gimp_sub_path_nbr ;
          gimp_sub_path_ind++ ) {
       gimp_sub_path_data= gimp_sub_path_arr[gimp_sub_path_ind];
       gimp_sub_path_bezier_curve_nbr= gimp_sub_path_data.gimp_sub_path_bezier_curve_nbr;
       gimp_sub_path_bezier_curve_arr= gimp_sub_path_data.gimp_sub_path_bezier_curve_arr;

       for ( gimp_sub_path_bezier_curve_ind= 0 ;
             gimp_sub_path_bezier_curve_ind< gimp_sub_path_bezier_curve_nbr ;
             gimp_sub_path_bezier_curve_ind++ ) {
          gimp_sub_path_bezier_curve_data= gimp_sub_path_bezier_curve_arr[gimp_sub_path_bezier_curve_ind];
          for ( control_point_ind= 0 ;
                control_point_ind< 4 ;
                control_point_ind++ ) {
             for ( k= 0 ; k< 2 ; k++ )
              control_point_arr[control_point_ind][k]= gimp_sub_path_bezier_curve_data.control_point_arr[control_point_ind][k];
          }
       }
    }
 }

}
