#include "header.h"

void alias_dense_depthmap_image_at_edge_image(
 int width,
 int height,
 int *dense_depthmap_image_arr,
 int *edge_image_arr,
 int *edge_alpha_arr
)

 /*
 Any pixel in the edge image
 should take the disparity of its neighbor
 if the neighbor is not in the edge image and is more in the foreground
 */

{

 int i;
 int j;
 int pixel;
 int depth;
 int i_radius;
 int j_radius;
 int i2;
 int j2;
 int pixel2;
 int depth2;
 int edge_alph;
 int edge_alph2;

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;

       /*
       Let's see if pixel is part of the edge image
       */

       edge_alph= edge_alpha_arr[pixel];

       if ( edge_alph == 0 ) {

          /*
          Pixel is not part of the edge image
          */

          continue;
       }
       if ( edge_alph == 255 ) {

          /*
          Pixel is part of the edge image
          */
       }

       depth= dense_depthmap_image_arr[pixel];
       for ( i_radius= -1 ; i_radius<= +1 ; i_radius++ ) {
          for ( j_radius= -1 ; j_radius<= +1 ; j_radius++ ) {
             i2= i+i_radius;
             j2= j+j_radius;
             if ( i2 < 0 )
              continue;
             if ( i2 > height-1 )
              continue;
             if ( j2 < 0 )
              continue;
             if ( j2 > width-1 )
              continue;
             pixel2= i2*width+j2;

             if ( pixel2 == pixel )
              continue;

             /*
             Let's see if pixel2 is part of the edge image
             */

             edge_alph2= edge_alpha_arr[pixel2];

             if ( edge_alph2 == 0 ) {

                /*
                Pixel2 is not part of the edge image
                */
             }
             if ( edge_alph2 == 255 ) {

                /*
                Pixel2 is part of the edge image
                */

                continue;
             }
       
             depth2= dense_depthmap_image_arr[pixel2];
             if ( !(depth2 > depth) )
              continue;
             dense_depthmap_image_arr[pixel]= depth2;
             depth= dense_depthmap_image_arr[pixel];
          }
       }
    }
 }

}
