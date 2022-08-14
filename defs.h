typedef struct gimp_sub_path_bezier_curve_struct {
 double control_point_arr[4][2];
} gimp_sub_path_bezier_curve_struct;

typedef struct gimp_sub_path_struct {
 int gimp_sub_path_bezier_curve_nbr;
 gimp_sub_path_bezier_curve_struct *gimp_sub_path_bezier_curve_arr;
} gimp_sub_path_struct;

typedef struct gimp_path_struct {
 char gimp_path_id[MAXLINE];
 int gimp_sub_path_nbr;
 gimp_sub_path_struct *gimp_sub_path_arr;
} gimp_path_struct;
