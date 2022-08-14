# the3dconverter2

Written and tested on Ubuntu 22.04.

Given an image, semi-automatically creates a depth map given depth clues (in the form of Gimp paths).

Inspiration for the3dconverter2 comes from:

"Deep Annotations: Designing Depth of a Single Image for Depth-based Effects" by Jingtang Liao, Shuheng Shen, Elmar Eisemann.

I liked some of their ideas but not all. I added my own ideas as well. 

To create the executable, compile the code in directory "the3dconverter2" using "make -f Makefile_g/Makefile_O" and then go into the "main" directory and create the exec using "make".

Test cases are given in the "test" directory.

Info about the3dconverter2 (theory behind it and how to use it) can be found here:

[2d to 3d Image Conversion Software - the3dconverter2](https://3dstereophoto.blogspot.com/2019/11/2d-to-3d-image-conversion-software-3d.html)

Dependencies (check the Makefiles):

"common" repo

libsuitesparse-dev
