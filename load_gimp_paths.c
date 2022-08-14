#include "header.h"

void load_gimp_paths(
 char *filename,
 int *pgimp_path_nbr,
 gimp_path_struct **pgimp_path_arr
)

{

 int gimp_path_nbr;
 gimp_path_struct *gimp_path_arr;
 char *char_ptr;
 char *char_ptr2;
 char line[MAXLINE];
 char line2[MAXLINE];
 char gimp_path_id[MAXLINE];
 FILE *fp;
 int gimp_path_ind;
 unsigned int char_ind;
 unsigned int char_ind2;
 int gimp_sub_path_ind;
 double control_point_arr[4][2];
 int gimp_sub_path_bezier_curve_ind;
 int k;
 int control_point_ind;

 gimp_path_nbr= 0;
 gimp_path_arr= 0;

 fp= fopen(filename,"r");
 if ( fp == NULL ) {
    goto END;
 }

 GIMP_PATH:

 /*
 Look for line that contains "<path id="
 That signals the beginning of a new gimp path
 */

 while ((char_ptr= fgets(line,MAXLINE,fp)) != NULL ) {
    char_ptr2= strstr(line,"<path id=");
    if ( char_ptr2 != NULL )
     break;
 }

 if ( char_ptr == NULL ) {

    /*
    Looks like we are done!
    */

    goto END;
 }

 /*
 Let's create a new gimp path
 */

 if ( gimp_path_nbr == 0 ) {
    gimp_path_arr= (gimp_path_struct *)calloc((gimp_path_nbr+1),sizeof(gimp_path_struct));
 }
 else {
    gimp_path_arr= (gimp_path_struct *)realloc(gimp_path_arr,(gimp_path_nbr+1)*sizeof(gimp_path_struct));
 }
 gimp_path_ind= gimp_path_nbr;
 gimp_path_nbr++;

 gimp_path_arr[gimp_path_ind].gimp_sub_path_nbr= 0;
 gimp_path_arr[gimp_path_ind].gimp_sub_path_arr= 0;

 for ( char_ind= 0 ; char_ind< strlen(line) ; char_ind++ ) {
    if ( line[char_ind] == '"' )
     break;
 }
 for ( char_ind2= 0 ; char_ind2< strlen(line) ; char_ind2++ ) {
    if ( line[char_ind+1+char_ind2] == '"' )
     break;
    gimp_path_id[char_ind2]= line[char_ind+1+char_ind2]; 
 }
 gimp_path_id[char_ind2]= '\0';

 strcpy(gimp_path_arr[gimp_path_ind].gimp_path_id,gimp_path_id);

 /*
 Look for line that contains "d=\"M "
 */

 while ((char_ptr= fgets(line,MAXLINE,fp)) != NULL ) {
    char_ptr2= strstr(line,"d=\"M ");
    if ( char_ptr2 != NULL )
     break;
 }

 if ( char_ptr == NULL ) {

    /*
    Looks like we are done!
    */

    goto END;
 }

 GIMP_SUB_PATH:

 /*
 Let's create a new gimp sub path
 */

 if ( gimp_path_arr[gimp_path_ind].gimp_sub_path_nbr == 0 ) {
    gimp_path_arr[gimp_path_ind].gimp_sub_path_arr= (gimp_sub_path_struct *)calloc((gimp_path_arr[gimp_path_ind].gimp_sub_path_nbr+1),sizeof(gimp_sub_path_struct));
 }
 else {
    gimp_path_arr[gimp_path_ind].gimp_sub_path_arr= (gimp_sub_path_struct *)realloc(gimp_path_arr[gimp_path_ind].gimp_sub_path_arr,(gimp_path_arr[gimp_path_ind].gimp_sub_path_nbr+1)*sizeof(gimp_sub_path_struct));
 }
 gimp_sub_path_ind= gimp_path_arr[gimp_path_ind].gimp_sub_path_nbr;
 gimp_path_arr[gimp_path_ind].gimp_sub_path_nbr++;

 gimp_path_arr[gimp_path_ind].gimp_sub_path_arr[gimp_sub_path_ind].gimp_sub_path_bezier_curve_nbr= 0;
 gimp_path_arr[gimp_path_ind].gimp_sub_path_arr[gimp_sub_path_ind].gimp_sub_path_bezier_curve_arr= 0;

 /*
 Let's extract control point 0 of gimp sub path bezier curve
 It's to the immediate right of "M "
 */

 for ( char_ind= 0 ; char_ind< strlen(line) ; char_ind++ ) {
    if ( line[char_ind] == 'M' )
     break;
 }
 char_ind++;

 sscanf(&line[char_ind],"%lg%c%lg",
  &control_point_arr[0][0],
  &line2[0],
  &control_point_arr[0][1]
 );

 GIMP_SUB_PATH_BEZIER_CURVE:

 fgets(line,MAXLINE,fp);

 /*
 Let's extract control point 1, 2, and 3 of gimp sub path bezier curve
 */

 char_ptr= strstr(line,"C ");
 if ( char_ptr == NULL ) {
    sscanf(line,"%lg%c%lg%lg%c%lg%lg%c%lg",
     &control_point_arr[1][0],
     &line2[0],
     &control_point_arr[1][1],
     &control_point_arr[2][0],
     &line2[1],
     &control_point_arr[2][1],
     &control_point_arr[3][0],
     &line2[2],
     &control_point_arr[3][1]
    );
 }
 else {
    for ( char_ind= 0 ; char_ind< strlen(line) ; char_ind++ ) {
       if ( line[char_ind] == 'C' )
        break;
    }
    char_ind++;
    sscanf(&line[char_ind],"%lg%c%lg%lg%c%lg%lg%c%lg",
     &control_point_arr[1][0],
     &line2[0],
     &control_point_arr[1][1],
     &control_point_arr[2][0],
     &line2[1],
     &control_point_arr[2][1],
     &control_point_arr[3][0],
     &line2[2],
     &control_point_arr[3][1]
    );
 }

 /*
 Let's create a new gimp sub path bezier curve
 */

 if ( gimp_path_arr[gimp_path_ind].gimp_sub_path_arr[gimp_sub_path_ind].gimp_sub_path_bezier_curve_nbr == 0 ) {
    gimp_path_arr[gimp_path_ind].gimp_sub_path_arr[gimp_sub_path_ind].gimp_sub_path_bezier_curve_arr= (gimp_sub_path_bezier_curve_struct *)calloc((gimp_path_arr[gimp_path_ind].gimp_sub_path_arr[gimp_sub_path_ind].gimp_sub_path_bezier_curve_nbr+1),sizeof(gimp_sub_path_bezier_curve_struct));
 }
 else {
    gimp_path_arr[gimp_path_ind].gimp_sub_path_arr[gimp_sub_path_ind].gimp_sub_path_bezier_curve_arr= (gimp_sub_path_bezier_curve_struct *)realloc(gimp_path_arr[gimp_path_ind].gimp_sub_path_arr[gimp_sub_path_ind].gimp_sub_path_bezier_curve_arr,(gimp_path_arr[gimp_path_ind].gimp_sub_path_arr[gimp_sub_path_ind].gimp_sub_path_bezier_curve_nbr+1)*sizeof(gimp_sub_path_bezier_curve_struct));
 }
 gimp_sub_path_bezier_curve_ind= gimp_path_arr[gimp_path_ind].gimp_sub_path_arr[gimp_sub_path_ind].gimp_sub_path_bezier_curve_nbr;
 gimp_path_arr[gimp_path_ind].gimp_sub_path_arr[gimp_sub_path_ind].gimp_sub_path_bezier_curve_nbr++;

 /*
 Store the control points for this gimp sub path bezier curve
 */

 for ( control_point_ind= 0 ;
       control_point_ind< 4 ;
       control_point_ind++ ) {
    for ( k= 0 ; k< 2 ; k++ )
     gimp_path_arr[gimp_path_ind].gimp_sub_path_arr[gimp_sub_path_ind].gimp_sub_path_bezier_curve_arr[gimp_sub_path_bezier_curve_ind].control_point_arr[control_point_ind][k]= control_point_arr[control_point_ind][k];
 }

 /*
 Let's see if we are done with this gimp path
 */

 char_ptr= strstr(line,"\" />");
 if ( char_ptr == NULL ) {

    /*
    We are not done with this gimp path
    */
 }
 else {

    /*
    We are done with this gimp path
    */

    goto GIMP_PATH;
 }

 /*
 Let's see if we are done with this gimp sub path
 looking for a "M " in the current line
 */

 char_ptr= strstr(line,"M ");
 if ( char_ptr == NULL ) {

    /*
    We are not done with this gimp sub path
    */
 }
 else {

    /*
    We are done with this gimp sub path
    */

    goto GIMP_SUB_PATH;
 }

 /*
 Let's see if we are done with this gimp sub path
 looking for a " Z" in the current line 
 */

 char_ptr= strstr(line," Z");
 if ( char_ptr == NULL ) {

    /*
    We are not done with this gimp sub path
    */
 }
 else {

    /*
    We are done with this gimp sub path
    */

    fgets(line,MAXLINE,fp);

    goto GIMP_SUB_PATH;
 }

 /*
 If here,
 we are not done with this gimp sub path
 */

 for ( k= 0 ; k< 2 ; k++ )
  control_point_arr[0][k]= control_point_arr[3][k];

 goto GIMP_SUB_PATH_BEZIER_CURVE;

 END:

 if ( fp != NULL ) {
    fclose(fp);
 }

 (*pgimp_path_nbr)= gimp_path_nbr;
 (*pgimp_path_arr)= gimp_path_arr;

}
