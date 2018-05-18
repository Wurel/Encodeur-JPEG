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
#include <unistd.h>


int main(int argc, char const *argv[])
{
    printf("Au boulot!\n");
    struct mcu **tableau_de_mcu;
    uint32_t *tab_taille = taille_tableau(argv[1],1,1);
    // Découpage
    tableau_de_mcu = decoupage_mc(argv[1],1,1);
// RGB -> YCbCr, DCT
    for (size_t i = 0; i < tab_taille[0]; i++) {
      for (size_t j = 0; j < tab_taille[1]; j++) {
        //rgb 2 ycbcr
        transformation_bloc_rgb_ycbcr(&tableau_de_mcu[i][j].tableau_de_bloc[0]);
        // DCT
        tableau_de_mcu[i][j].tableau_de_bloc_apres_dct = malloc(sizeof(struct bloc_apres_dct));
        tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].y = malloc(sizeof(uint16_t)*64);
        dct_bloc(tableau_de_mcu[i][j].tableau_de_bloc[0].y, tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].y);
        //Zigzag
        tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].y =
                                zigzag_composante(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].y);
        //Quantification
        quantification_composante(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].y);
      }
    }
    for (size_t i = 0; i < 8; i++) {
      for (size_t j = 0; j < 8; j++) {
        printf("%x\t", tableau_de_mcu[1][1].tableau_de_bloc_apres_dct[0].y[j+i*8]);
      }
      printf("\n");
    }

    struct jpeg_desc *jpeg = jpeg_desc_create();
    jpeg_desc_set_ppm_filename(jpeg, argv[1]);
    jpeg_desc_set_jpeg_filename(jpeg, strcat(argv[1], "jpeg"));
    jpeg_desc_set_nb_components(jpeg, 1);
    jpeg_desc_set_image_width(jpeg, tab_taille[0]*8); // atttention h1 V1
    jpeg_desc_set_image_height(jpeg, tab_taille[1]*8);
    jpeg_desc_set_sampling_factor(jpeg, Y, H, 1);
    jpeg_desc_set_sampling_factor(jpeg, Y, V, 1);
    jpeg_desc_set_huffman_table(jpeg, DC, Y, huffman_table_build(htables_nb_symb_per_lengths[0][0], htables_symbols[0][0], htables_nb_symbols[0][0]));
    jpeg_desc_set_huffman_table(jpeg, DC, Cb, huffman_table_build(htables_nb_symb_per_lengths[0][1], htables_symbols[0][1], htables_nb_symbols[0][1]));
    jpeg_desc_set_huffman_table(jpeg, AC, Y, huffman_table_build(htables_nb_symb_per_lengths[1][0], htables_symbols[1][0], htables_nb_symbols[1][0]));
    jpeg_desc_set_huffman_table(jpeg, AC, Cb, huffman_table_build(htables_nb_symb_per_lengths[1][1], htables_symbols[1][1], htables_nb_symbols[1][1]));
    jpeg_desc_set_quantization_table(jpeg, Y, compressed_Y_table);
    jpeg_write_header(jpeg);
    struct bitstream *bits; //= bitstream_create("/user/6/.base/poraa/home/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/test.jpeg");
    bits = jpeg_desc_get_bitstream(jpeg);
    // ecriture_symbole_DC(bits, tableau_de_mcu[0][0].tableau_de_bloc_apres_dct[0].y[0]);
    printf("%d\n", tab_taille[1]);
    ecriture_AC_DC_complete(bits, tableau_de_mcu, tab_taille[0], tab_taille[1]);
    // AC_composante_puis_huffman(bits, tableau_de_mcu[0][0].tableau_de_bloc_apres_dct[0].y);
    jpeg_write_footer(jpeg);
    return EXIT_SUCCESS;
}
