#include "header.h"

void create_dense_diffusion_direction_image_main(
 int width,
 int height,
 int *diffusion_direction_image_arr,
 int *diffusion_direction_alpha_arr,
 double *dense_diffusion_direction_theta_image_arr,
 int gimp_path_nbr,
 gimp_path_struct *gimp_path_arr,
 int *edge_image_arr,
 int *edge_alpha_arr
)

{

 double *sparse_diffusion_direction_theta_image_arr;
 int *sparse_diffusion_direction_theta_alpha_arr;

 sparse_diffusion_direction_theta_image_arr= (double *)calloc(width*height,sizeof(double));

 sparse_diffusion_direction_theta_alpha_arr= (int *)calloc(width*height,sizeof(int));

 compute_diffusion_direction_from_gimp_paths(
  width,
  height,
  gimp_path_nbr,
  gimp_path_arr,
  sparse_diffusion_direction_theta_image_arr,
  sparse_diffusion_direction_theta_alpha_arr
 );

 create_dense_diffusion_direction_image(
  width,
  height,
  diffusion_direction_image_arr,
  diffusion_direction_alpha_arr,
  sparse_diffusion_direction_theta_image_arr,
  sparse_diffusion_direction_theta_alpha_arr,
  dense_diffusion_direction_theta_image_arr
 );

 free(sparse_diffusion_direction_theta_image_arr);

 free(sparse_diffusion_direction_theta_alpha_arr);

}
