#include <stdlib.h>
#include <stdio.h>
#include "decoupage_mcu.h"

int main(void)
{
    printf("Au boulot!\n");
    struct mcu **tab;
    tab = decoupage_mc("/user/6/poraa/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/images/gris.pgm",1,1);
    printf("%x\n", tab[0][0].rgb[3]);
    return EXIT_SUCCESS;
}
