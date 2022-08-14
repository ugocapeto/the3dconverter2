#include "header.h"

void print_gimp_paths(
 int gimp_path_nbr,
 gimp_path_struct *gimp_path_arr,
 char *filename
)

{

 FILE *fp;
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

 fp= fopen(filename,"w");

 for ( gimp_path_ind= 0 ;
       gimp_path_ind< gimp_path_nbr ;
       gimp_path_ind++ ) {
    gimp_path_data= gimp_path_arr[gimp_path_ind];
    gimp_sub_path_nbr= gimp_path_data.gimp_sub_path_nbr;
    gimp_sub_path_arr= gimp_path_data.gimp_sub_path_arr;
    fprintf(fp,"gimp path name = %s\n",gimp_path_data.gimp_path_id);
    for ( gimp_sub_path_ind= 0 ;
          gimp_sub_path_ind< gimp_sub_path_nbr ;
          gimp_sub_path_ind++ ) {
       gimp_sub_path_data= gimp_sub_path_arr[gimp_sub_path_ind];
       gimp_sub_path_bezier_curve_nbr= gimp_sub_path_data.gimp_sub_path_bezier_curve_nbr;
       gimp_sub_path_bezier_curve_arr= gimp_sub_path_data.gimp_sub_path_bezier_curve_arr;
       fprintf(fp,"gimp_sub_path_ind = %d\n",gimp_sub_path_ind);
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
          fprintf(fp,"%.2f,%.2f %.2f,%.2f %.2f,%.2f %.2f,%.2f\n",
           control_point_arr[0][0],control_point_arr[0][1],
           control_point_arr[1][0],control_point_arr[1][1],
           control_point_arr[2][0],control_point_arr[2][1],
           control_point_arr[3][0],control_point_arr[3][1]
          );
       }
    }
 }

 fclose(fp);

}
