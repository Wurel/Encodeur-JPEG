#include <stdlib.h>
#include <stdio.h>
#include "decoupage_mcu.h"
#include "dct.h"
#include "rgb2ycbcr.h"
#include "q_zz.h"
#include "AC_DC.h"

int main(void)
{
    printf("Au boulot!\n");
    struct mcu **tab;
    tab = decoupage_mc("/home/marion/Encodeur-JPEG/etu/images/invader.pgm",1,1);
    transformation_bloc_rgb_ycbcr(&tab[0][0].tableau_de_bloc[0]);
    struct bloc_apres_dct blc;
    blc.y = malloc(sizeof(uint16_t)*64);
    // printf("youhou");
    dct_bloc(tab[0][0].tableau_de_bloc[0].y, blc.y);
    int16_t * tabl;
    tabl = zigzag_composante(blc.y);
    quantification_composante(tabl);
    // for (size_t i = 0; i < 8; i++) {
    //   for (size_t j = 0; j < 8; j++) {
    //     printf("%x\t", tabl[j+8*i]);
    //   }
    //   printf("\n");
    // }
    // printf("%x\n", blc.y[0]);
    retourne_magnitude_bits(-4);
    return EXIT_SUCCESS;
}
