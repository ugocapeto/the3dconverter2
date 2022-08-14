#include "header.h"

void create_dense_diffusion_direction_image(
 int width,
 int height,
 int *diffusion_direction_image_arr,
 int *diffusion_direction_alpha_arr,
 double *sparse_diffusion_direction_theta_image_arr,
 int *sparse_diffusion_direction_theta_alpha_arr,
 double *dense_diffusion_direction_theta_image_arr
)

{

 int i;
 int j;
 unsigned int pixel;
 int i_radius;
 int j_radius;
 int i2;
 int j2;
 unsigned int pixel2;
 double value;
 double pixel_coeff_value;
 double pixel2_coeff_value;
 double b_value;
 int radius= 1;
 unsigned int k;
 unsigned int row_ind;
 unsigned int col_ind;
 int alpha;
 int alpha2;
 int diffusion_direction_alpha;
 double theta;
 double theta2;
 int diffusion_direction_alpha2;

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

       /*
       Let's see if pixel belongs to an area
       where the diffusion should follow directionality
       */

       diffusion_direction_alpha= diffusion_direction_alpha_arr[pixel];
       if ( diffusion_direction_alpha == 0 ) {

          /*
          Pixel does not belongs to an area
          where the diffusion should follow directionality
          */

          /*
          Do as if diffusion direction at pixel was known
          */

          /*
          Fill matrix and right-hand side vector
          */

          value= 1.0;
          if ( value != 0 ) {
             if ( !(pixel >= 0 && pixel < nrow) ) {
                error_handler((char *)"create_dense_diffusion_direction_image");
             }
             if ( !(pixel >= 0 && pixel < ncol) ) {
                error_handler((char *)"create_dense_diffusion_direction_image");
             }
             triplet_i[triplet->nnz] = pixel;
             triplet_j[triplet->nnz] = pixel;
             triplet_x[triplet->nnz] = value;
             triplet->nnz++;
          }

          value= 0.0;
          if ( value != 0 ) {
             ((double *)(b->x))[pixel] = value;
          }

          /*
          We are done for that pixel
          */

          continue;
       }

       theta= sparse_diffusion_direction_theta_image_arr[pixel];
       alpha= sparse_diffusion_direction_theta_alpha_arr[pixel];

       /*
       If alpha==  0, the diffusion direction is unknown
       If alpha==255, the diffusion direction is   known
       and is equal to sparse_diffusion_direction_theta_image_arr[pixel]
       */

       if ( alpha == 255 ) {

          /*
          Diffusion direction at pixel is known
          */

          /*
          Fill matrix and right-hand side vector
          */

          value= 1.0;
          if ( value != 0 ) {
             if ( !(pixel >= 0 && pixel < nrow) ) {
                error_handler((char *)"create_dense_diffusion_direction_image");
             }
             if ( !(pixel >= 0 && pixel < ncol) ) {
                error_handler((char *)"create_dense_diffusion_direction_image");
             }
             triplet_i[triplet->nnz] = pixel;
             triplet_j[triplet->nnz] = pixel;
             triplet_x[triplet->nnz] = value;
             triplet->nnz++;
          }

          value= theta;
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
       diffusion direction at pixel is unknown
       */

       /*
       Fill matrix and right-hand side vector
       */

       pixel_coeff_value= 0.0;

       b_value= 0.0;

       for ( i_radius= -radius ; i_radius<= +radius ; i_radius++ ) {
          for ( j_radius= -radius ; j_radius<= +radius ; j_radius++ ) {
             if ( !(abs(i_radius+j_radius)%2 == 1) )
              continue;
             i2= i+i_radius;
             j2= j+j_radius;
             pixel2= i2*width+j2;

             /*
             Check if neighboring pixel is out of bounds
             */

             if ( i2 < 0 )
              continue;
             if ( i2 > height-1 )
              continue;
             if ( j2 < 0 )
              continue;
             if ( j2 > width-1 )
              continue;

             /*
             Neighboring pixel is in bounds
             */

             /*
             Let's see if pixel2 belongs to an area
             where the diffusion should follow directionality
             */

             diffusion_direction_alpha2= diffusion_direction_alpha_arr[pixel2];
             if ( diffusion_direction_alpha2 == 0 ) {

                /*
                Pixel2 doesn not belong to an area
                where the diffusion should follow directionality
                */

                /*
                Do not consider pixel2 in the linear equation for pixel
                */

                continue;
             }

             theta2= sparse_diffusion_direction_theta_image_arr[pixel2];
             alpha2= sparse_diffusion_direction_theta_alpha_arr[pixel2];

             /*
             If alpha2==  0, the diffusion direction is unknown
             If alpha2==255, the diffusion direction is   known
             and is equal to sparse_diffusion_direction_theta_image_arr[pixel2]
             */

             if ( alpha2 == 0 ) {

                /*
                Diffusion direction at pixel2 is unknown
                */

                pixel2_coeff_value= -1.;

                value= pixel2_coeff_value;
                if ( value != 0 ) {
                   if ( !(pixel >= 0 && pixel < nrow) ) {
                      error_handler((char *)"create_dense_diffusion_direction_image");
                   }
                   if ( !(pixel2 >= 0 && pixel2 < ncol) ) {
                      error_handler((char *)"create_dense_diffusion_direction_image");
                   }
                   triplet_i[triplet->nnz] = pixel;
                   triplet_j[triplet->nnz] = pixel2;
                   triplet_x[triplet->nnz] = value;
                   triplet->nnz++;
                }
             }
             if ( alpha2 == 255 ) {

                /*
                Diffusion direction at pixel2 is known
                */

                b_value+= theta2;
             }

             pixel_coeff_value+= 1.;
          }
       }

       value= pixel_coeff_value;
       if ( value != 0 ) {
          if ( !(pixel >= 0 && pixel < nrow) ) {
             error_handler((char *)"create_dense_diffusion_direction_image");
          }
          if ( !(pixel >= 0 && pixel < ncol) ) {
             error_handler((char *)"create_dense_diffusion_direction_image");
          }
          triplet_i[triplet->nnz] = pixel;
          triplet_j[triplet->nnz] = pixel;
          triplet_x[triplet->nnz] = value;
          triplet->nnz++;
       }

       value= b_value;
       if ( value != 0 ) {
          ((double *)(b->x))[pixel] = value;
       }
    }
 }

 if ( !(triplet->nnz <= nzmax) ) {
    error_handler((char *)"create_dense_diffusion_direction_image");
 }

 for ( k= 0 ; k< triplet->nnz ; k++ ) {
    row_ind= triplet_i[k];
    col_ind= triplet_j[k];
    value= triplet_x[k];
    if ( !(row_ind >= 0 && row_ind < nrow) ) {
       error_handler((char *)"create_dense_diffusion_direction_image");
    }
    if ( !(col_ind >= 0 && col_ind < ncol) ) {
       error_handler((char *)"create_dense_diffusion_direction_image");
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
 Fill the dense diffusion direction image
 */

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;

       /*
       Let's see if pixel belongs to an area
       where the diffusion should follow directionality
       */

       diffusion_direction_alpha= diffusion_direction_alpha_arr[pixel];
       if ( diffusion_direction_alpha == 0 ) {

          /*
          Pixel does not belongs to an area
          where the diffusion should follow directionality
          */

          /*
          Do nothing
          */

          continue;
       }

       theta= sparse_diffusion_direction_theta_image_arr[pixel];
       alpha= sparse_diffusion_direction_theta_alpha_arr[pixel];

       /*
       If alpha==  0, the diffusion direction is unknown
       If alpha==255, the diffusion direction is   known
       and is equal to sparse_diffusion_direction_theta_image_arr[pixel]
       */

       if ( alpha == 0 ) {

          /*
          Diffusion direction at pixel is unknown
          */

          value= ((double *)(x->x))[pixel];
          theta= value;
       }
       if ( alpha == 255 ) {

          /*
          Diffusion direction at pixel is known
          */
       }

       /*
       Store the diffusion direction in the dense diffusion direction image
       */

       dense_diffusion_direction_theta_image_arr[pixel]= theta;
    }
 }

 // Free memory
 cholmod_l_free_dense(&residual, &common);
 cholmod_l_free_sparse(&A, &common);
 cholmod_l_free_dense(&x, &common);
 cholmod_l_free_dense(&b, &common);
 cholmod_l_finish(&common);

}
