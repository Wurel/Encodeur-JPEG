#ifndef rgb2ycbcr
#define rgb2ycbcr


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "structures.h"

/*
  transforme UNE mcu (son pointeur est mis en parametre) en RGB en une mcu en YCbCr
*/
extern void transformation_bloc_rgb_ycbcr(struct bloc *blo);


#endif /* rgb2ycbcr */
