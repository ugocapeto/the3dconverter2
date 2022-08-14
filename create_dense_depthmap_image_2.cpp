#include "header.h"

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
)

{

 int i_radius;
 int j_radius;
 int i2;
 int j2;
 unsigned int pixel2;
 int depth2;
 double value;
 double pixel_coeff_value;
 double pixel2_coeff_value;
 double b_value;
 int radius= 1;
 int alpha2;
 int edge_alph2;

 size_t nrow = width*height ;
 size_t ncol = width*height ;

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

       depth2= sparse_depthmap_image_arr[pixel2];
       alpha2= sparse_depthmap_alpha_arr[pixel2];

       /*
       If alpha2==  0, the depth is unknown
       If alpha2==255, the depth is   known
       and is equal to sparse_depthmap_image_arr[pixel2]
       */

       /*
       Let's see if pixel2 is part of the edge image
       */

       edge_alph2= edge_alpha_arr[pixel2]; 

       if ( edge_alph2 == 0 ) {

          /*
          Pixel2 is not part of the edge image
          */

          /*
          Pixel2 will contribute to the diffusion equation for pixel
          */
       }
       if ( edge_alph2 == 255 ) {

          /*
          Pixel2 is part of the edge image
          */

          /*
          Pixel2 will not contribute to the diffusion equation for pixel
          */

          continue;
       }

       if ( alpha2 == 0 ) {

          /*
          Depth at pixel2 is unknown
          */

          pixel2_coeff_value= -1.;

          value= pixel2_coeff_value;
          if ( value != 0 ) {
             if ( !(pixel >= 0 && pixel < nrow) ) {
                error_handler((char *)"create_dense_depthmap_image_2");
             }
             if ( !(pixel2 >= 0 && pixel2 < ncol) ) {
                error_handler((char *)"create_dense_depthmap_image_2");
             }
             triplet_i[triplet->nnz] = pixel;
             triplet_j[triplet->nnz] = pixel2;
             triplet_x[triplet->nnz] = value;
             triplet->nnz++;
          }
       }
       if ( alpha2 == 255 ) {

          /*
          Depth at pixel2 is known
          */

          if ( !(depth2 >= 0 && depth2 <= 255) ) {
             error_handler((char *)"create_dense_depthmap_image_2");
          }

          b_value+= (double)depth2;
       }

       pixel_coeff_value+= 1.;
    }
 }

 value= pixel_coeff_value;
 if ( value != 0 ) {
    if ( !(pixel >= 0 && pixel < nrow) ) {
       error_handler((char *)"create_dense_depthmap_image_2");
    }
    if ( !(pixel >= 0 && pixel < ncol) ) {
       error_handler((char *)"create_dense_depthmap_image_2");
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
