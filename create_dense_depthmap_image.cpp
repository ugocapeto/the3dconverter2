#include "header.h"

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
)

{

 int i;
 int j;
 unsigned int pixel;
 int depth;
 double value;
 unsigned int k;
 unsigned int row_ind;
 unsigned int col_ind;
 int alpha;
 int diffusion_direction_alpha;
 int method;
 int i2;
 int j2;

 /*
 Ready to solve with suitesparse
 */

 cholmod_common common;
 cholmod_sparse *A;
 cholmod_dense *x, *b, *residual;
 double residual_norm;
 double one[] = { 1, 0 }, minusone[] = { -1, 0 };
 
 cholmod_l_start(&common);

 size_t nrow = width*height ;
 size_t ncol = width*height ;

 /*
 Initialize x and b
 */

 x = cholmod_l_zeros(ncol, 1, CHOLMOD_REAL, &common);
 b = cholmod_l_zeros(nrow, 1, CHOLMOD_REAL, &common);

 /*
 Allocate memory for the triplets
 nzmax (maximum number of triplets) should be as low as possible
 */

 size_t nnz_row = 5 ; //max number of non-zero entries on a row
 size_t nzmax = nrow*nnz_row ;
 cholmod_triplet *triplet = cholmod_l_allocate_triplet(nrow, ncol, nzmax, 0, CHOLMOD_REAL, &common);
 if (triplet == nullptr) {
   perror("cholmod_l_allocate_triplet");
   abort();
 }
 LONG* triplet_i = (LONG *)(triplet->i);
 LONG* triplet_j = (LONG *)(triplet->j);
 double * triplet_x = (double *)(triplet->x);

 /*
 Fill matrix and right-hand side vector
 */

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;

       /*
       pixel represents the row in matrix A
       We are looking at the linear equation for pixel
       */

       depth= sparse_depthmap_image_arr[pixel];
       alpha= sparse_depthmap_alpha_arr[pixel];
       diffusion_direction_alpha= diffusion_direction_alpha_arr[pixel];

       /*
       If alpha==  0, the depth is unknown
       If alpha==255, the depth is   known
       and is equal to sparse_depthmap_image_arr[pixel]
       */

       if ( alpha == 255 ) {

          /*
          Depth at pixel is known
          */

          if ( !(depth >= 0 && depth <= 255) ) {
             error_handler((char *)"create_dense_depthmap_image");
          }

          /*
          Fill matrix and right-hand side vector
          */

          value= 1.0;
          if ( value != 0 ) {
             if ( !(pixel >= 0 && pixel < nrow) ) {
                error_handler((char *)"create_dense_depthmap_image");
             }
             if ( !(pixel >= 0 && pixel < ncol) ) {
                error_handler((char *)"create_dense_depthmap_image");
             }
             triplet_i[triplet->nnz] = pixel;
             triplet_j[triplet->nnz] = pixel;
             triplet_x[triplet->nnz] = value;
             triplet->nnz++;
          }

          value= (double)depth;
          if ( value != 0 ) {
             ((double *)(b->x))[pixel] = value;
          }

          /*
          We are done for that pixel
          */

          continue;
       }

       /*
       If here,
       depth at pixel is unknown
       */

       /*
       Fill matrix and right-hand side vector
       for equation corresponding to row = pixel
       */

       /*
       If the pixel is not wihin a direction diffusion zone
       (diffusion_direction_alpha = 0),
       we are gonna use the linear equation with no built-in directionality
       If the pixel is wihin a direction diffusion zone
       (diffusion_direction_alpha = 255),
       we are gonna use the linear equation with built-in directionality
       */ 

       method= 2;
       if ( diffusion_direction_alpha == 255 ) {
          method= 3;
          i2= i-1;
          if ( !(i2 >= 0) ) {
             method= 2;
          }
          i2= i+1;
          if ( !(i2 < height) ) {
             method= 2;
          }
          j2= j-1;
          if ( !(j2 >= 0) ) {
             method= 2;
          }
          j2= j+1;
          if ( !(j2 < width) ) {
             method= 2;
          }
       }

       if ( method == 2 ) { 

          /*
          For debugging purposes,
          do as if depth at pixel wass known
          Comment out when done debugging
          */

          /*
          value= 1.0;
          if ( value != 0 ) {
             if ( !(pixel >= 0 && pixel < nrow) ) {
                error_handler((char *)"create_dense_depthmap_image");
             }
             if ( !(pixel >= 0 && pixel < ncol) ) {
                error_handler((char *)"create_dense_depthmap_image");
             }
             triplet_i[triplet->nnz] = pixel;
             triplet_j[triplet->nnz] = pixel;
             triplet_x[triplet->nnz] = value;
             triplet->nnz++;
          }

          value= (double)0;
          if ( value != 0 ) {
             ((double *)(b->x))[pixel] = value;
          }

          continue;
          */

          create_dense_depthmap_image_2(
           reference_rgb_image_arr,
           width,
           height,
           sparse_depthmap_image_arr,
           sparse_depthmap_alpha_arr,
           dense_depthmap_image_arr,
           edge_image_arr,
           edge_alpha_arr,
           i,
           j,
           pixel,
           triplet,
           triplet_i,
           triplet_j,
           triplet_x,
           b
          );
       }
       if ( method == 3 ) {  

          /*
          For debugging purposes,
          do as if depth at pixel wass known
          Comment out when done debugging
          */

          /*
          value= 1.0;
          if ( value != 0 ) {
             if ( !(pixel >= 0 && pixel < nrow) ) {
                error_handler((char *)"create_dense_depthmap_image");
             }
             if ( !(pixel >= 0 && pixel < ncol) ) {
                error_handler((char *)"create_dense_depthmap_image");
             }
             triplet_i[triplet->nnz] = pixel;
             triplet_j[triplet->nnz] = pixel;
             triplet_x[triplet->nnz] = value;
             triplet->nnz++;
          }

          value= (double)0;
          if ( value != 0 ) {
             ((double *)(b->x))[pixel] = value;
          }

          continue;
          */

          create_dense_depthmap_image_3(
           reference_rgb_image_arr,
           width,
           height,
           sparse_depthmap_image_arr,
           sparse_depthmap_alpha_arr,
           dense_depthmap_image_arr,
           dense_diffusion_direction_theta_image_arr,
           edge_image_arr,
           edge_alpha_arr,
           i,
           j,
           pixel,
           triplet,
           triplet_i,
           triplet_j,
           triplet_x,
           b
          );
       }
    }
 }

 if ( !(triplet->nnz <= nzmax) ) {
    error_handler((char *)"create_dense_depthmap_image");
 }

 for ( k= 0 ; k< triplet->nnz ; k++ ) {
    row_ind= triplet_i[k];
    col_ind= triplet_j[k];
    value= triplet_x[k];
    if ( !(row_ind >= 0 && row_ind < nrow) ) {
       error_handler((char *)"create_dense_depthmap_image");
    }
    if ( !(col_ind >= 0 && col_ind < ncol) ) {
       error_handler((char *)"create_dense_depthmap_image");
    }
 }

 // Convert triplet to sparse matrix
 A = cholmod_l_triplet_to_sparse(triplet, triplet->nnz, &common);
 if (A == nullptr) {
   perror("cholmod_l_triplet_to_sparse");
   abort();
 }
 cholmod_l_free_triplet(&triplet, &common);

 // Solve
 x = SuiteSparseQR<double>(A, b, &common);

 // Compute the residual
 residual = cholmod_l_copy_dense(b, &common);
 cholmod_l_sdmult(A, 0, minusone, one, x, residual, &common);
 residual_norm = cholmod_l_norm_dense(residual, 2, &common) ;

 std::cout << "|| A x - b ||_2 = " << residual_norm << "\n";

 /*
 Fill the dense depthmap image
 */

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;

       depth= sparse_depthmap_image_arr[pixel];
       alpha= sparse_depthmap_alpha_arr[pixel];

       /*
       If alpha==  0, the depth is unknown
       If depth==255, the depth is   known
       and is equal to sparse_depthmap_image_arr[pixel]
       */

       if ( alpha == 0 ) {

          /*
          Depth at pixel is unknown
          */

          value= ((double *)(x->x))[pixel];
          depth= (int)(value+0.5);
          if ( depth < 0 )
           depth= 0;
          if ( depth > 255 )
           depth= 255;
       }
       if ( alpha == 255 ) {

          /*
          Depth at pixel is known
          */

          if ( !(depth >= 0 && depth <= 255) ) {
             error_handler((char *)"create_dense_depthmap_image");
          }
       }

       /*
       Store the depth in the dense depthmap image
       */

       dense_depthmap_image_arr[pixel]= depth;
    }
 }

 // Free memory
 cholmod_l_free_dense(&residual, &common);
 cholmod_l_free_sparse(&A, &common);
 cholmod_l_free_dense(&x, &common);
 cholmod_l_free_dense(&b, &common);
 cholmod_l_finish(&common);

}
