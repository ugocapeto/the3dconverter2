#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define MAXLINE 256
#include <limits.h>

#include "io/proto.h"
#include "math/proto.h"
#include "util/proto.h"

/*
Add stuff to support SuiteSparseQR (begin)
*/

#include <cstdlib>

#include <iostream>

#include <SuiteSparseQR.hpp>

#ifdef __linux__
 //linux
 typedef long LONG;
 typedef unsigned long UNSIGNED_LONG;
#elif _WIN32
 //windows
 typedef long long LONG;
 typedef unsigned long long UNSIGNED_LONG;
#endif

/*
Add stuff to support SuiteSparseQR (end)
*/

#include "defs.h"
#include "proto.h"
