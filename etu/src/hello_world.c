#include <stdlib.h>
#include <stdio.h>
#include "decoupage_mcu.h"
#include "q_zz.h"

int main(void)
{
    printf("Au boulot!\n");
    struct mcu **tab;
    tab = decoupage_mc("/user/6/poraa/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/images/invader.pgm",1,1);
    transformation_rgb_ycbcr(&tab[0][0]);
    struct bloc_apres_dct blc;
    // for (size_t i = 0; i < 64; i++) {
    //   printf("%hhx\n", tab[0][0].y[i]);
    // }
    printf("youhou");
    dct(tab[0][0].y, blc.y);
    // for (size_t i = 0; i < 64; i++) {
    //   printf("%hx\n", blc.y[i]);
    // }
    // printf("%x\n", blc.y[0]);
    zigzag_composante()
    return EXIT_SUCCESS;
}
