#include <stdlib.h>
#include <stdio.h>
#include "decoupage_mcu.h"
#include "q_zz.h"

int main(void)
{
    printf("Au boulot!\n");
    struct mcu **tab;
    tab = decoupage_mc("/user/6/poraa/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/images/invader.pgm",1,1);
    tab[0][0].tableau_de_bloc[0].y = malloc(64*sizeof(uint8_t));
    tab[0][0].tableau_de_bloc[0].cb = malloc(64*sizeof(uint8_t));
    tab[0][0].tableau_de_bloc[0].cr = malloc(64*sizeof(uint8_t));
    transformation_bloc_rgb_ycbcr(&tab[0][0].tableau_de_bloc[0]);
    for (size_t i = 0; i < 8; i++) {
      for (size_t j = 0; j < 8; j++) {
        printf("%x\t", tab[0][0].tableau_de_bloc[0].rgb[j+8*i]);
      }
      printf("\n");
    }
    // for (size_t i = 0; i < 64; i++) {
    //   printf("%hhx\n", tab[0][0].y[i]);
    // }
    struct bloc_apres_dct blc;
    blc.y = malloc(sizeof(uint16_t)*64);
    // printf("youhou");
    dct(tab[0][0].tableau_de_bloc[0].y, blc.y);
    printf("coucou1\n");
    int16_t * tabl;
    tabl = zigzag_composante(blc.y);
    quantification_composante(tabl);
    // printf("%x\n", blc.y[0]);
    return EXIT_SUCCESS;
}
