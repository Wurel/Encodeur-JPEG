#ifndef decoupage_mcu
#define decoupage_mcu

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
/*structure representant le mcu*/

struct mcu;



extern struct mcu **decoupage_mcu(const char *ppm_filename, int8_t h1, int8_t v1);

/* permet de recuperer les échantillons RGB à partir du fichier
 * d’entrée PPM premiere case type du tableau deuxieme case largeur troisieme case
 * hauteur */
extern uint8_t *recuperation_rgb(const char *ppm_filename);

extern struct mcu transformation_rgb_ycbcr(struct mcu *mc);

extern struct mcu dct(struct mcu *mc);

#endif /* decoupage_mcu */
