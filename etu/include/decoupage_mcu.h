#ifndef decoupage_mcu
#define decoupage_mcu

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
/*structure representant le mcu*/

struct mcu{
  //en vrai c'est un bloc
  uint8_t h;
  uint8_t v;
  uint32_t rgb[64];
  uint8_t y[64];
  uint8_t cb[64];
  uint8_t cr[64];
};

struct bloc_apres_dct{
    int16_t y[64];
    int16_t cb[64];
    int16_t cr[64];
};

extern struct mcu **decoupage_mc(const char *ppm_filename, int8_t h1, int8_t v1);

/* permet de recuperer les échantillons RGB à partir du fichier
 * d’entrée PPM premiere case type du tableau deuxieme case largeur troisieme case
 * hauteur */
extern uint8_t *recuperation_rgb(const char *ppm_filename);

extern void transformation_rgb_ycbcr(struct mcu *mc);

extern void dct(uint8_t *composante, int16_t *nouvelle_composante);

#endif /* decoupage_mcu */
