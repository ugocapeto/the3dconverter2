#include "header.h"

void create_dense_depthmap_image_main(
 int *reference_rgb_image_arr,
 int width,
 int height,
 int *sparse_depthmap_image_arr,
 int *sparse_depthmap_alpha_arr,
 int *dense_depthmap_image_arr,
 int gimp_path_nbr,
 gimp_path_struct *gimp_path_arr,
 int *diffusion_direction_image_arr,
 int *diffusion_direction_alpha_arr,
 double *dense_diffusion_direction_theta_image_arr,
 int *edge_image_arr,
 int *edge_alpha_arr
)

{

 int *sparse_depthmap_rgb_image_arr;
 int i;
 int j;
 int pixel;
 int cind;
 int err_flag;

 add_gimp_paths_to_sparse_depthmap_image(
  width,
  height,
  sparse_depthmap_image_arr,
  sparse_depthmap_alpha_arr,
  gimp_path_nbr,
  gimp_path_arr
 );

 sparse_depthmap_rgb_image_arr= (int *)calloc(width*height,3*sizeof(int));

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       for ( cind= 0 ; cind< 3 ; cind++ )
        sparse_depthmap_rgb_image_arr[3*pixel+cind]= sparse_depthmap_image_arr[pixel];
    }
 }

 err_flag= write_rgba_image(
  (char *)"sparse_depthmap_rgba_image_2.png",
  sparse_depthmap_rgb_image_arr,
  sparse_depthmap_alpha_arr,
  width,
  height
 );

 free(sparse_depthmap_rgb_image_arr);

 create_dense_depthmap_image(
  reference_rgb_image_arr,
  width,
  height,
  sparse_depthmap_image_arr,
  sparse_depthmap_alpha_arr,
  dense_depthmap_image_arr,
  diffusion_direction_image_arr,
  diffusion_direction_alpha_arr,
  dense_diffusion_direction_theta_image_arr,
  edge_image_arr,
  edge_alpha_arr
 );

 err_flag= write_image(
  (char *)"dense_depthmap_image_2.png",
  dense_depthmap_image_arr,
  width,
  height
 );

 alias_dense_depthmap_image_at_edge_image(
  width,
  height,
  dense_depthmap_image_arr,
  edge_image_arr,
  edge_alpha_arr
 );

}
