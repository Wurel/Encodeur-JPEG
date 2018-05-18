#include <stdlib.h>
#include <stdio.h>
#include "decoupage_mcu.h"
#include "dct.h"
#include "rgb2ycbcr.h"
#include "q_zz.h"
#include "AC_DC.h"
#include "jpeg_writer.h"
#include "htables.h"
#include "qtables.h"
#include "bitstream.h"
#include "huffman.h"

int main(void)
{
    printf("Au boulot!\n");
    struct mcu **tab;
    uint32_t *tab_taille = taille_tableau("/user/6/.base/poraa/home/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/images/invader.pgm",1,1);
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

    // struct bitstream *bit = bitstream_create("/user/6/.base/poraa/home/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/test");
    // ecriture(bit, 4);

    // struct huff_table table = huffman_table_build()

    struct jpeg_desc *jpeg = jpeg_desc_create();
    jpeg_desc_set_ppm_filename(jpeg, "/user/6/.base/poraa/home/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/images/invader.pgm");
    jpeg_desc_set_jpeg_filename(jpeg, "/user/6/.base/poraa/home/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/test.jpeg");
    jpeg_desc_set_nb_components(jpeg, 1);
    jpeg_desc_set_image_width(jpeg, tab_taille[0]*8); // atttention h1 V1
    jpeg_desc_set_image_height(jpeg, tab_taille[1]*8);
    jpeg_desc_set_sampling_factor(jpeg, Y, H, 1);
    jpeg_desc_set_sampling_factor(jpeg, Y, V, 1);
    // jpeg_desc_set_huffman_table()
    jpeg_desc_set_huffman_table(jpeg, DC, Y, huffman_table_build(htables_nb_symb_per_lengths[1][0], htables_symbols[1][0], htables_nb_symbols[1][0]));
    jpeg_desc_set_quantization_table(jpeg, Y, compressed_Y_table);
    // struct bitstream *bits = bitstream_create("/user/6/.base/poraa/home/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/test.jpeg");
    // bits = jpeg_desc_get_bitstream(jpeg);
    // printf("slt %d\n", jpeg_desc_get_image_height(jpeg));
    // printf("coucou\n");
    jpeg_write_header(jpeg);
    jpeg_write_footer(jpeg);
    return EXIT_SUCCESS;
}
