#ifndef down_sampler
#define down_sampler

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "structures.h"

extern void echantillonnage_horizontal(struct mcu, uint8_t facteur);

extern void echantillonnage_vertical(struct mcu, uint8_t facteur);

#endif /* down_sampler */
