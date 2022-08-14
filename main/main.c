#include "header.h"

int main(
 int argc,
 char* argv[]
)

{

 FILE *fp;
 char filename[MAXLINE];
 int err_flag;
 int width;
 int height;
 int nbr_pixels;
 int i;
 int j;
 int r;
 int g;
 int b;
 double val_dbl;
 int val_int;
 int pixel;
 int count;
 int alpha;
 char filename_reference_rgb_image[MAXLINE];
 char filename_sparse_depthmap_rgba_image[MAXLINE];
 char filename_dense_depthmap_image[MAXLINE];
 int *reference_rgb_image_arr;
 int *sparse_depthmap_rgb_image_arr;
 int *sparse_depthmap_a_image_arr;
 int *sparse_depthmap_image_arr;
 int *dense_depthmap_image_arr;
 int *sparse_depthmap_alpha_arr;
 int gimp_path_nbr;
 gimp_path_struct *gimp_path_arr;
 char filename_gimp_paths[MAXLINE];
 char filename_diffusion_direction_rgba_image[MAXLINE];
 int *diffusion_direction_rgb_image_arr;
 int *diffusion_direction_a_image_arr;
 int *diffusion_direction_image_arr;
 int *diffusion_direction_alpha_arr;
 double *dense_diffusion_direction_theta_image_arr;
 char filename_edge_rgba_image[MAXLINE];
 int *edge_rgb_image_arr;
 int *edge_a_image_arr;
 int *edge_image_arr;
 int *edge_alpha_arr;

 /*
 Let's read in the input file
 */

 fp= fopen("the3dconverter2_input.txt","r");

 /*
 Get filename for reference rgb image
 */

 fscanf(fp,"%s",filename);

 fprintf(stdout,"reference rgb image = %s\n",filename);

 strcpy(filename_reference_rgb_image,filename);

 /*
 Get filename for sparse depthmap rgba image
 */

 fscanf(fp,"%s",filename);

 fprintf(stdout,"sparse depthmap rgba image = %s\n",filename);

 strcpy(filename_sparse_depthmap_rgba_image,filename);

 /*
 Get filename for dense depthmap image
 */

 fscanf(fp,"%s",filename);

 fprintf(stdout,"dense depthmap image = %s\n",filename);

 strcpy(filename_dense_depthmap_image,filename);

 /*
 Get filename for gimp paths
 */

 fscanf(fp,"%s",filename);

 fprintf(stdout,"gimp paths = %s\n",filename);

 strcpy(filename_gimp_paths,filename);

 /*
 Get filename for diffusion direction rgba image
 */

 fscanf(fp,"%s",filename);

 fprintf(stdout,"diffusion direction rgba image = %s\n",filename);

 strcpy(filename_diffusion_direction_rgba_image,filename);

 /*
 Get filename for edge rgba image
 */

 fscanf(fp,"%s",filename);

 fprintf(stdout,"edge rgba image = %s\n",filename);

 strcpy(filename_edge_rgba_image,filename);

 /*
 Done reading the input file
 */

 fclose(fp);

 /*
 Load reference rgb image
 */

 fp= fopen(filename_reference_rgb_image,"r");

 if ( fp == NULL ) {
    fprintf(stderr,"Filename %s does not exist!\n",filename_reference_rgb_image);
    return 1;
 }
 else {
    fclose(fp); 

    err_flag= load_rgb_image(
     filename_reference_rgb_image,
     &reference_rgb_image_arr,
     &width,
     &height
    );

    if ( err_flag == 1 ) {
       fprintf(stderr,"rgb image in filename %s could not be loaded!\n",filename_reference_rgb_image);
       return 1;
    }
 }

 /*
 Load sparse depthmap rgba image
 */

 fp= fopen(filename_sparse_depthmap_rgba_image,"r");

 if ( fp == NULL ) {
    nbr_pixels= width*height;
    sparse_depthmap_rgb_image_arr= (int *)calloc(nbr_pixels,3*sizeof(int));
    sparse_depthmap_a_image_arr= (int *)calloc(nbr_pixels,sizeof(int));
 }
 else {
    fclose(fp);

    err_flag= load_rgba_image(
     filename_sparse_depthmap_rgba_image,
     &sparse_depthmap_rgb_image_arr,
     &sparse_depthmap_a_image_arr,
     &width,
     &height
    );

    if ( err_flag == 1 ) {
       fprintf(stderr,"rgba image in filename %s could not be loaded!\n",filename_sparse_depthmap_rgba_image);
       return 1;
    }
 }

 /*
 Check to make sure all pixels are either
 fully opaque or fully transparent
 */

 count= 0;
 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       alpha= sparse_depthmap_a_image_arr[pixel];
       if ( alpha == 255 )
        continue;
       if ( alpha == 0 )
        continue;
       count++;
    }
 }
 if ( count > 0 ) {
    fprintf(stdout,"There are %d pixels in the sparse depthmap rgba image that are neither fully transparent nor fully opaque. In Gimp, use Layer->Transparency->Threshold Alpha to remove them.\n",count);
    return 1;
 }

 /*
 Check to make sure there are pixels that are transparent
 */

 count= 0;
 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       alpha= sparse_depthmap_a_image_arr[pixel];
       if ( alpha == 255 )
        continue;
       count++;
    }
 }
 if ( count == 0 ) {
    fprintf(stdout,"All pixels in the sparse depthmap rgba image are fully opaque. There needs to be transparent pixels otherwise nothing is going to happen.\n");
    return 1;
 }

 /*
 Transfer the info
 from sparse_depthmap_rgb_image_arr and sparse_depthmap_a_image_arr
 to sparse_depthmap_image_arr and sparse_depthmap_alpha_arr
 if sparse_depthmap_alpha_arr[pixel] ==  0, the depth at pixel is unknown
 if sparse_depthmap_alpha_arr[pixel] ==255, the depth at pixel is   known
 and is equal to sparse_depthmap_image_arr[pixel]
 */

 nbr_pixels= width*height;
 sparse_depthmap_image_arr= (int *)calloc(nbr_pixels,sizeof(int));
 sparse_depthmap_alpha_arr= (int *)calloc(nbr_pixels,sizeof(int));

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       r= sparse_depthmap_rgb_image_arr[3*pixel+0];
       g= sparse_depthmap_rgb_image_arr[3*pixel+1];
       b= sparse_depthmap_rgb_image_arr[3*pixel+2];
       val_dbl= (6969.*(double)r+23434.*(double)g+2365.*(double)b)/32768.;
       val_int= (int)(val_dbl+0.5);
       if ( val_int < 0 )
        val_int= 0;
       if ( val_int > 255 )
        val_int= 255;
       alpha= sparse_depthmap_a_image_arr[pixel];
       sparse_depthmap_image_arr[pixel]= val_int;
       sparse_depthmap_alpha_arr[pixel]= alpha;
    }
 }

 /*
 Free sparse_depthmap_rgb_image_arr
 */

 free(sparse_depthmap_rgb_image_arr);

 /*
 Free sparse_depthmap_a_image_arr
 */

 free(sparse_depthmap_a_image_arr);

 /*
 Load the gimp paths
 */

 load_gimp_paths(
  filename_gimp_paths,
  &gimp_path_nbr,
  &gimp_path_arr
 );

 /*
 Print the gimp paths
 */

 print_gimp_paths(
  gimp_path_nbr,
  gimp_path_arr,
  (char *)"gimp_paths.txt"
 );

 /*
 Check the gimp paths
 */

 check_gimp_paths(
  gimp_path_nbr,
  gimp_path_arr
 );

 /*
 Load diffusion direction rgba image
 */

 fp= fopen(filename_diffusion_direction_rgba_image,"r");

 if ( fp == NULL ) {
    nbr_pixels= width*height;
    diffusion_direction_rgb_image_arr= (int *)calloc(nbr_pixels,3*sizeof(int));
    diffusion_direction_a_image_arr= (int *)calloc(nbr_pixels,sizeof(int));
 }
 else {
    fclose(fp);

    err_flag= load_rgba_image(
     filename_diffusion_direction_rgba_image,
     &diffusion_direction_rgb_image_arr,
     &diffusion_direction_a_image_arr,
     &width,
     &height
    );

    if ( err_flag == 1 ) {
       fprintf(stderr,"rgba image in filename %s could not be loaded!\n",filename_diffusion_direction_rgba_image);
       return 1;
    }
 }

 /*
 Check to make sure all pixels are either
 fully opaque or fully transparent
 */

 count= 0;
 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       alpha= diffusion_direction_a_image_arr[pixel];
       if ( alpha == 255 )
        continue;
       if ( alpha == 0 )
        continue;
       count++;
    }
 }
 if ( count > 0 ) {
    fprintf(stdout,"There are %d pixels in the diffusion direction rgba image that are neither fully transparent nor fully opaque. In Gimp, use Layer->Transparency->Threshold Alpha to remove them.\n",count);
    return 1;
 }

 /*
 Check to make sure there are pixels that are transparent
 */

 count= 0;
 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       alpha= diffusion_direction_a_image_arr[pixel];
       if ( alpha == 255 )
        continue;
       count++;
    }
 }
 if ( count == 0 ) {
    fprintf(stdout,"All pixels in the diffusion direction rgba image are fully opaque. There needs to be transparent pixels otherwise nothing is going to happen.\n");
    return 1;
 }

 /*
 Transfer the info
 from diffusion_direction_rgb_image_arr and diffusion_direction_a_image_arr
 to diffusion_direction_image_arr and diffusion_direction_alpha_arr
 if diffusion_direction_alpha_arr[pixel] ==   0, the pixel is not part of the diffusion direction image
 if diffusion_direction_alpha_arr[pixel] == 255, the pixel is     part of the diffusion direction image
 */

 nbr_pixels= width*height;
 diffusion_direction_image_arr= (int *)calloc(nbr_pixels,sizeof(int));
 diffusion_direction_alpha_arr= (int *)calloc(nbr_pixels,sizeof(int));

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       r= diffusion_direction_rgb_image_arr[3*pixel+0];
       g= diffusion_direction_rgb_image_arr[3*pixel+1];
       b= diffusion_direction_rgb_image_arr[3*pixel+2];
       val_dbl= (6969.*(double)r+23434.*(double)g+2365.*(double)b)/32768.;
       val_int= (int)(val_dbl+0.5);
       if ( val_int < 0 )
        val_int= 0;
       if ( val_int > 255 )
        val_int= 255;
       alpha= diffusion_direction_a_image_arr[pixel];
       diffusion_direction_image_arr[pixel]= val_int;
       diffusion_direction_alpha_arr[pixel]= alpha;
    }
 }

 /*
 Free diffusion_direction_rgb_image_arr
 */

 free(diffusion_direction_rgb_image_arr);

 /*
 Free diffusion_direction_a_image_arr
 */

 free(diffusion_direction_a_image_arr);

 /*
 Load edge rgba image
 */

 fp= fopen(filename_edge_rgba_image,"r");

 if ( fp == NULL ) {
    nbr_pixels= width*height;
    edge_rgb_image_arr= (int *)calloc(nbr_pixels,3*sizeof(int));
    edge_a_image_arr= (int *)calloc(nbr_pixels,sizeof(int));
 }
 else {
    fclose(fp); 

    err_flag= load_rgba_image(
     filename_edge_rgba_image,
     &edge_rgb_image_arr,
     &edge_a_image_arr,
     &width,
     &height
    );

    if ( err_flag == 1 ) {
       fprintf(stderr,"rgba image in filename %s could not be loaded!\n",filename_edge_rgba_image);
       return 1;
    }
 }

 /*
 Check to make sure all pixels are either
 fully opaque or fully transparent
 */

 count= 0;
 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       alpha= edge_a_image_arr[pixel];
       if ( alpha == 255 )
        continue;
       if ( alpha == 0 )
        continue;
       count++;
    }
 }
 if ( count > 0 ) {
    fprintf(stdout,"There are %d pixels in the edge rgba image that are neither fully transparent nor fully opaque. In Gimp, use Layer->Transparency->Threshold Alpha to remove them.\n",count);
    return 1;
 }

 /*
 Check to make sure there are pixels that are transparent
 */

 count= 0;
 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       alpha= edge_a_image_arr[pixel];
       if ( alpha == 255 )
        continue;
       count++;
    }
 }
 if ( count == 0 ) {
    fprintf(stdout,"All pixels in the edge rgba image are fully opaque. There needs to be transparent pixels otherwise nothing is going to happen.\n");
    return 1;
 }

 /*
 Transfer the info
 from edge_rgb_image_arr and edge_a_image_arr
 to edge_image_arr aand edge_alpha_arr
 if edge_alpha_arr[pixel] ==   0, the pixel is not part of the edge image
 if edge_alpha_arr[pixel] == 255, the pixel is     part of the edge image
 */

 nbr_pixels= width*height;
 edge_image_arr= (int *)calloc(nbr_pixels,sizeof(int));
 edge_alpha_arr= (int *)calloc(nbr_pixels,sizeof(int));

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       r= edge_rgb_image_arr[3*pixel+0];
       g= edge_rgb_image_arr[3*pixel+1];
       b= edge_rgb_image_arr[3*pixel+2];
       val_dbl= (6969.*(double)r+23434.*(double)g+2365.*(double)b)/32768.;
       val_int= (int)(val_dbl+0.5);
       if ( val_int < 0 )
        val_int= 0;
       if ( val_int > 255 )
        val_int= 255;
       alpha= edge_a_image_arr[pixel];
       edge_image_arr[pixel]= val_int;
       edge_alpha_arr[pixel]= alpha;
    }
 }

 /*
 Free edge_rgb_image_arr
 */

 free(edge_rgb_image_arr);

 /*
 Free edge_gradient_a_image_arr
 */

 free(edge_a_image_arr);

 /*
 Create the dense diffusion direction theta image
 */

 nbr_pixels= width*height;
 dense_diffusion_direction_theta_image_arr= (double *)calloc(nbr_pixels,sizeof(double));

 fprintf(stdout,"Creating the dense diffusion direction theta image ...\n");

 create_dense_diffusion_direction_image_main(
  width,
  height,
  diffusion_direction_image_arr,
  diffusion_direction_alpha_arr,
  dense_diffusion_direction_theta_image_arr,
  gimp_path_nbr,
  gimp_path_arr,
  edge_image_arr,
  edge_alpha_arr
 );

 fprintf(stdout,"Creating the dense diffusion direction theta image ... done.\n");

 /*
 Create the dense depthmap image
 */

 nbr_pixels= width*height;
 dense_depthmap_image_arr= (int *)calloc(nbr_pixels,sizeof(int));

 fprintf(stdout,"Creating the dense depthmap image ...\n");

 create_dense_depthmap_image_main(
  reference_rgb_image_arr,
  width,
  height,
  sparse_depthmap_image_arr,
  sparse_depthmap_alpha_arr,
  dense_depthmap_image_arr,
  gimp_path_nbr,
  gimp_path_arr,
  diffusion_direction_image_arr,
  diffusion_direction_alpha_arr,
  dense_diffusion_direction_theta_image_arr,
  edge_image_arr,
  edge_alpha_arr
 );

 fprintf(stdout,"Creating the dense depthmap image ... done.\n");

 /*
 Print the output depth map
 */

 err_flag= write_image(
  filename_dense_depthmap_image,
  dense_depthmap_image_arr,
  width,
  height
 );

 if ( err_flag == 1 )
  return 1;

 /*
 Free reference_rgb_image_arr
 */

 free(reference_rgb_image_arr);

 /*
 Free sparse_depthmap_image_arr
 */

 free(sparse_depthmap_image_arr);

 /*
 Free sparse_depthmap_alpha_arr
 */

 free(sparse_depthmap_alpha_arr);

 /*
 Free dense_depthmap_image_arr
 */

 free(dense_depthmap_image_arr);

 /*
 Free the gimp paths
 */

 free_gimp_paths(
  gimp_path_nbr,
  gimp_path_arr
 );

 /*
 Free diffusion_direction_image_arr
 */

 free(diffusion_direction_image_arr);

 /*
 Free diffusion_direction_alpha_arr
 */

 free(diffusion_direction_alpha_arr);

 /*
 Free dense_diffusion_direction_theta_image_arr
 */

 free(dense_diffusion_direction_theta_image_arr);

 /*
 Free edge_image_arr
 */

 free(edge_image_arr);

 /*
 Free edge_alpha_arr
 */

 free(edge_alpha_arr);

 return 0;

}
