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
    uint32_t *tab_taille = taille_tableau("/user/6/.base/poraa/home/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/images/gris.pgm",1,1);
    printf("%d\n", tab_taille[0]);
    tab = decoupage_mc("/user/6/.base/poraa/home/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/images/invader.pgm",1,1);
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
    struct bitstream *bit = bitstream_create("/user/6/.base/poraa/home/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/test");
    ecriture(bit, 4);

    // struct huff_table table = huffman_table_build()

    return EXIT_SUCCESS;
}
