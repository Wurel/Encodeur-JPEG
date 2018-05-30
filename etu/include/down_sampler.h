#ifndef down_sampler
#define down_sampler

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "structures.h"

/*
  Effectue l'echantillonnage_horizontal si les facteur le permette (gerer dans le hello_word)
  Prend en parametre le tableau de mcu
*/
extern void echantillonnage_horizontal(struct mcu, uint8_t facteur);

/*
  Effectue l'echantillonnage_vertical si les facteur le permette (gerer dans le hello_word)
  Prend en parametre le tableau de mcu
*/
extern void echantillonnage_vertical(struct mcu, uint8_t facteur, uint8_t facteur_horizontal);

#endif /* down_sampler */
