#include "header.h"

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
)

{

 int i2;
 int j2;
 unsigned int pixel2;
 int depth2;
 double value;
 double pixel_coeff_value;
 double pixel2_coeff_value;
 double b_value;
 int alpha2;
 double theta;
 double pi= acos(-1.);
 double lambda= 1.0;

 size_t nrow = width*height ;
 size_t ncol = width*height ;

 theta= dense_diffusion_direction_theta_image_arr[pixel];

 pixel_coeff_value= 0.0;

 b_value= 0.0;

 i2= i;
 j2= j-1;
 pixel2= i2*width+j2;

 depth2= sparse_depthmap_image_arr[pixel2];
 alpha2= sparse_depthmap_alpha_arr[pixel2];

 /*
 If alpha2==  0, the depth is unknown
 If alpha2==255, the depth is   known
 and is equal to sparse_depthmap_image_arr[pixel2]
 */

 if ( alpha2 == 0 ) {

    /*
    Depth at pixel2 is unknown
    */

    pixel2_coeff_value= -1. * ( lambda * fabs(cos(theta)) + (1.-lambda) * fabs(cos(theta+pi/2.)) );
    /*
    pixel2_coeff_value= -1. * fabs(cos(theta));
    */

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

    b_value+= +1. * ( lambda * fabs(cos(theta)) + (1.-lambda) * fabs(cos(theta+pi/2.)) ) * (double)depth2;
    /*
    b_value+= +1. * fabs(cos(theta)) * (double)depth2;
    */
 }

 pixel_coeff_value+= +1. * ( lambda * fabs(cos(theta)) + (1.-lambda) * fabs(cos(theta+pi/2.)) );
 /*
 pixel_coeff_value+= +1. * fabs(cos(theta));
 */

 i2= i;
 j2= j+1;
 pixel2= i2*width+j2;

 depth2= sparse_depthmap_image_arr[pixel2];
 alpha2= sparse_depthmap_alpha_arr[pixel2];

 /*
 If alpha2==  0, the depth is unknown
 If alpha2==255, the depth is   known
 and is equal to sparse_depthmap_image_arr[pixel2]
 */

 if ( alpha2 == 0 ) {

    /*
    Depth at pixel2 is unknown
    */

    pixel2_coeff_value= -1. * ( lambda * fabs(cos(theta)) + (1.-lambda) * fabs(cos(theta+pi/2.)) );
    /*
    pixel2_coeff_value= -1. * fabs(cos(theta));
    */

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

    b_value+= +1. * ( lambda * fabs(cos(theta)) + (1.-lambda) * fabs(cos(theta+pi/2.)) ) * (double)depth2;
    /*
    b_value+= +1. * fabs(cos(theta)) * (double)depth2;
    */
 }

 pixel_coeff_value+= +1. * ( lambda * fabs(cos(theta)) + (1.-lambda) * fabs(cos(theta+pi/2.)) );
 /*
 pixel_coeff_value+= +1. * fabs(cos(theta));
 */

 i2= i-1;
 j2= j;
 pixel2= i2*width+j2;

 depth2= sparse_depthmap_image_arr[pixel2];
 alpha2= sparse_depthmap_alpha_arr[pixel2];

 /*
 If alpha2==  0, the depth is unknown
 If alpha2==255, the depth is   known
 and is equal to sparse_depthmap_image_arr[pixel2]
 */

 if ( alpha2 == 0 ) {

    /*
    Depth at pixel2 is unknown
    */

    pixel2_coeff_value= -1. * ( lambda * fabs(sin(theta)) + (1.-lambda) * fabs(sin(theta+pi/2.)) );
    /*
    pixel2_coeff_value= -1. * fabs(sin(theta));
    */

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

    b_value+= +1. * ( lambda * fabs(sin(theta)) + (1.-lambda) * fabs(sin(theta+pi/2.)) ) * (double)depth2;
    /*
    b_value+= +1. * fabs(sin(theta)) * (double)depth2;
    */
 }

 pixel_coeff_value+= +1. * ( lambda * fabs(sin(theta)) + (1.-lambda) * fabs(sin(theta+pi/2.)) );
 /*
 pixel_coeff_value+= +1. * fabs(sin(theta));
 */

 i2= i+1;
 j2= j;
 pixel2= i2*width+j2;

 depth2= sparse_depthmap_image_arr[pixel2];
 alpha2= sparse_depthmap_alpha_arr[pixel2];

 /*
 If alpha2==  0, the depth is unknown
 If alpha2==255, the depth is   known
 and is equal to sparse_depthmap_image_arr[pixel2]
 */

 if ( alpha2 == 0 ) {

    /*
    Depth at pixel2 is unknown
    */

    pixel2_coeff_value= -1. * ( lambda * fabs(sin(theta)) + (1.-lambda) * fabs(sin(theta+pi/2.)) );
    /*
    pixel2_coeff_value= -1. * fabs(sin(theta));
    */

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

    b_value+= +1. * ( lambda * fabs(sin(theta)) + (1.-lambda) * fabs(sin(theta+pi/2.)) ) * (double)depth2;
    /*
    b_value+= +1. * fabs(sin(theta)) * (double)depth2;
    */
 }

 pixel_coeff_value+= +1. * ( lambda * fabs(sin(theta)) + (1.-lambda) * fabs(sin(theta+pi/2.)) );
 /*
 pixel_coeff_value+= +1. * fabs(sin(theta));
 */

 value= pixel_coeff_value;
 if ( value != 0 ) {
    if ( !(pixel >= 0 && pixel < nrow) ) {
       error_handler((char *)"create_dense_depthmap_image_3");
    }
    if ( !(pixel >= 0 && pixel < ncol) ) {
       error_handler((char *)"create_dense_depthmap_image_3");
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
