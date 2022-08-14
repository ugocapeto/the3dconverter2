include main_objects.h
#
PROGRAM = the3dconverter2
#
CC = g++
CFLAGS = -g -Wall\
-I.. -I../../common\
-std=c++11\
-I/usr/include/suitesparse
#
all: $(PROGRAM)
#
$(PROGRAM): $(main_objects) ../the3dconverter2.o
	$(CC) -o $@ $(main_objects)\
        ../the3dconverter2.o\
        ../../common/io/io.o\
        ../../common/jpeg/jpeg.o\
        ../../common/math/math.o\
        ../../common/png/png.o\
        ../../common/tiff/tiff.o\
        ../../common/util/util.o\
        -ltiff\
        -lpng\
        -ljpeg\
        -lz\
        -lcholmod\
        -lspqr\
        -lm
#
.c.o:
	$(CC) -c $(CFLAGS) $<
