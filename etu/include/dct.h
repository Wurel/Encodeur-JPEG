#ifndef dct
#define dct

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "decoupage_mcu.h"


/*
  Transformation en cosinus discrete (DCT):
  prend en parametre la composante a transformer ainsi qu'une composante vide
  (qui deviendra la composante transformee)
*/
extern void dct_bloc(uint8_t *composante, int16_t *nouvelle_composante);


#endif /* dct */
