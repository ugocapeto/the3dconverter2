void add_gimp_paths_to_sparse_depthmap_image(
 int width,
 int height,
 int *sparse_depthmap_image_arr,
 int *sparse_depthmap_alpha_arr,
 int gimp_path_nbr,
 gimp_path_struct *gimp_path_arr
);

void alias_dense_depthmap_image_at_edge_image(
 int width,
 int height,
 int *dense_depthmap_image_arr,
 int *edge_image_arr,
 int *edge_alpha_arr
);

void check_gimp_paths(
 int gimp_path_nbr,
 gimp_path_struct *gimp_path_arr
);

void compute_diffusion_direction_from_gimp_paths(
 int width,
 int height,
 int gimp_path_nbr,
 gimp_path_struct *gimp_path_arr,
 double *sparse_diffusion_direction_theta_image_arr,
 int *sparse_diffusion_direction_theta_alpha_arr
);

void create_dense_depthmap_image_2(
 int *reference_rgb_image_arr,
 int width,
 int height,
 int *sparse_depthmap_image_arr,
 int *sparse_depthmap_alpha_arr,
 int *dense_depthmap_image_arr,
 int *edge_image_arr,
 int *edge_alpha_arr,
 int i,
 int j,
 unsigned int pixel,
 cholmod_triplet *triplet,
 LONG* triplet_i,
 LONG* triplet_j,
 double * triplet_x,
 cholmod_dense *b
);

void create_dense_depthmap_image_3(
 int *reference_rgb_image_arr,
 int width,
 int height,
 int *sparse_depthmap_image_arr,
 int *sparse_depthmap_alpha_arr,
 int *dense_depthmap_image_arr,
 double *dense_diffusion_direction_theta_image_arr,
 int *edge_image_arr,
 int *edge_alpha_arr,
 int i,
 int j,
 unsigned int pixel,
 cholmod_triplet *triplet,
 LONG* triplet_i,
 LONG* triplet_j,
 double * triplet_x,
 cholmod_dense *b
);

void create_dense_depthmap_image(
 int *reference_rgb_image_arr,
 int width,
 int height,
 int *sparse_depthmap_image_arr,
 int *sparse_depthmap_alpha_arr,
 int *dense_depthmap_image_arr,
 int *diffusion_direction_image_arr,
 int *diffusion_direction_alpha_arr,
 double *dense_diffusion_direction_theta_image_arr,
 int *edge_image_arr,
 int *edge_alpha_arr
);

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
);

void create_dense_diffusion_direction_image(
 int width,
 int height,
 int *diffusion_direction_image_arr,
 int *diffusion_direction_alpha_arr,
 double *sparse_diffusion_direction_theta_image_arr,
 int *sparse_diffusion_direction_theta_alpha_arr,
 double *dense_diffusion_direction_theta_image_arr
);

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
);

void free_gimp_paths(
 int gimp_path_nbr,
 gimp_path_struct *gimp_path_arr
);

void load_gimp_paths(
 char *filename,
 int *pgimp_paths_nbr,
 gimp_path_struct **pgimp_paths_arr
);

void plot_line(
 int x0,
 int y0,
 int x1,
 int y1,
 int **pxy_arr,
 int *pxy_nbr
);

void plot_line_high(
 int x0,
 int y0,
 int x1,
 int y1,
 int **pxy_arr,
 int *pxy_nbr
);

void plot_line_low(
 int x0,
 int y0,
 int x1,
 int y1,
 int **pxy_arr,
 int *pxy_nbr
);

void print_gimp_paths(
 int gimp_path_nbr,
 gimp_path_struct *gimp_path_arr,
 char *filename
);
