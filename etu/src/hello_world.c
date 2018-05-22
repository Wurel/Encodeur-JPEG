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
    uint8_t tipe = type(argv[1]);
    printf("%d\n", tipe);
    struct mcu **tableau_de_mcu;
    uint32_t *tab_taille = malloc(3*sizeof(uint32_t));
    tab_taille = taille_tableau(argv[1],1,1);
    uint32_t *tab_taille_x8 = malloc(3*sizeof(uint32_t));
    tab_taille_x8 = taille_tableau_x8(argv[1], 1, 1);
    uint32_t largeur = tab_taille_x8[0];
    uint32_t hauteur = tab_taille_x8[1];
    free(tab_taille_x8);
    uint8_t *tab_rgb_rembourre = malloc((ajustement_taille(largeur)*ajustement_taille(hauteur)+3)*sizeof(uint8_t));
    tab_rgb_rembourre = rgb_rembourre(argv[1], 1, 1);
    // Découpage
    // printf("%d\n", largeur);
    // // printf("%d\n", ajustement_taille(largeur));
    // for (size_t i = 0; i < 376; i++) {
    //   for (size_t j = 0; j < 367; j++) {
    //     printf("[%d, %d] %x\t", i, j,tab_rgb_rembourre[i*ajustement_taille(largeur)+j+3]);
    //   }
    //   printf("\n");
    //   printf("\n");
    // }
    tableau_de_mcu = decoupage_mc(argv[1],1,1);
// RGB -> YCbCr, DCT
    for (uint32_t i = 0; i < ajustement_taille(hauteur)/8; i++) {
      for (uint32_t j = 0; j < ajustement_taille(largeur)/8; j++) {
        //rgb 2 ycbcr
        transformation_bloc_rgb_ycbcr(&tableau_de_mcu[i][j].tableau_de_bloc[0]);
        // DCT
        tableau_de_mcu[i][j].tableau_de_bloc_apres_dct = malloc(sizeof(struct bloc_apres_dct));
        tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].y = malloc(sizeof(int16_t)*64);
        dct_bloc(tableau_de_mcu[i][j].tableau_de_bloc[0].y, tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].y);

        tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].cb = malloc(sizeof(int16_t)*64);
        dct_bloc(tableau_de_mcu[i][j].tableau_de_bloc[0].cb, tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].cb);

        tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].cr = malloc(sizeof(int16_t)*64);
        dct_bloc(tableau_de_mcu[i][j].tableau_de_bloc[0].cr, tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].cr);

        // // //Zigzag
        tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].y =
                                zigzag_composante(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].y);

        tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].cb =
                                zigzag_composante(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].cb);

        tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].cr =
                                zigzag_composante(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].cr);
        //
        // //Quantification
        //
        quantification_Y(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].y);
        quantification_Cb_Cr(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].cb);
        quantification_Cb_Cr(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].cr);

      }
    }

    // for (uint32_t i = 0; i < 8; i++) {
    //   for (uint32_t j = 0; j < 8; j++) {
    //     printf("%x\t", tableau_de_mcu[0][0].tableau_de_bloc_apres_dct[0].y[j+i*8]);
    //   }
    //   printf("\n");
    // }

    //
    //
    // //
    // printf("dct %d\n", tableau_de_mcu[0][0].tableau_de_bloc_apres_dct[0].y[0]);
    struct jpeg_desc *jpeg = jpeg_desc_create();
    jpeg_desc_set_ppm_filename(jpeg, argv[1]);
    jpeg_desc_set_jpeg_filename(jpeg, strcat(argv[1], "jpeg"));
    jpeg_desc_set_nb_components(jpeg, tipe);
    jpeg_desc_set_image_width(jpeg, largeur); // atttention h1 V1
    jpeg_desc_set_image_height(jpeg, hauteur);

    jpeg_desc_set_sampling_factor(jpeg, Y, H, 1);
    jpeg_desc_set_sampling_factor(jpeg, Y, V, 1);
    jpeg_desc_set_huffman_table(jpeg, DC, Y, huffman_table_build(htables_nb_symb_per_lengths[0][0], htables_symbols[0][0], htables_nb_symbols[0][0]));
    // jpeg_desc_set_huffman_table(jpeg, DC, Cb, huffman_table_build(htables_nb_symb_per_lengths[0][1], htables_symbols[0][1], htables_nb_symbols[0][1]));
    jpeg_desc_set_huffman_table(jpeg, AC, Y, huffman_table_build(htables_nb_symb_per_lengths[1][0], htables_symbols[1][0], htables_nb_symbols[1][0]));
    // jpeg_desc_set_huffman_table(jpeg, AC, Cb, huffman_table_build(htables_nb_symb_per_lengths[1][1], htables_symbols[1][1], htables_nb_symbols[1][1]));
    jpeg_desc_set_quantization_table(jpeg, Y, compressed_Y_table);

    if (tipe == 3) {
      jpeg_desc_set_sampling_factor(jpeg, Cb, H, 1);
      jpeg_desc_set_sampling_factor(jpeg, Cb, V, 1);
      jpeg_desc_set_huffman_table(jpeg, DC, Cb, huffman_table_build(htables_nb_symb_per_lengths[0][1], htables_symbols[0][1], htables_nb_symbols[0][1]));
      jpeg_desc_set_huffman_table(jpeg, AC, Cb, huffman_table_build(htables_nb_symb_per_lengths[1][1], htables_symbols[1][1], htables_nb_symbols[1][1]));
      jpeg_desc_set_quantization_table(jpeg, Cb, compressed_CbCr_table);

      jpeg_desc_set_sampling_factor(jpeg, Cr, H, 1);
      jpeg_desc_set_sampling_factor(jpeg, Cr, V, 1);
      jpeg_desc_set_huffman_table(jpeg, DC, Cr, huffman_table_build(htables_nb_symb_per_lengths[0][2], htables_symbols[0][2], htables_nb_symbols[0][2]));
      jpeg_desc_set_huffman_table(jpeg, AC, Cr, huffman_table_build(htables_nb_symb_per_lengths[1][2], htables_symbols[1][2], htables_nb_symbols[1][2]));
      jpeg_desc_set_quantization_table(jpeg, Cr, compressed_CbCr_table);
    }

    jpeg_write_header(jpeg);
    struct bitstream *bits; //= bitstream_create("/user/6/.base/poraa/home/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/test.jpeg");
    bits = jpeg_desc_get_bitstream(jpeg);
    // ecriture_symbole_DC(bits, tableau_de_mcu[0][0].tableau_de_bloc_apres_dct[0].y[0]);
    printf("%d %d \n", ajustement_taille(largeur)/8, ajustement_taille(hauteur)/8);
    ecriture_AC_DC_complete(bits, tableau_de_mcu, ajustement_taille(hauteur)/8, ajustement_taille(largeur)/8, tipe);
    jpeg_write_footer(jpeg);
    jpeg_desc_destroy(jpeg);
    //free tableau_de_mcu

    for (uint32_t i = 0; i < tab_taille[1]; i++) {
      for (uint32_t j = 0; j < tab_taille[0]; j++) {
        free(tableau_de_mcu[i][j].tableau_de_bloc[0].rgb);
        free(tableau_de_mcu[i][j].tableau_de_bloc[0].y);
        free(tableau_de_mcu[i][j].tableau_de_bloc[0].cb);
        free(tableau_de_mcu[i][j].tableau_de_bloc[0].cr);
        free(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].y);
        free(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].cb);
        free(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0].cr);
        free(tableau_de_mcu[i][j].tableau_de_bloc);
        free(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct);
      }
    }
    for (uint32_t i = 0; i <tab_taille[1]; i++) {
      free(tableau_de_mcu[i]);
    }
    free(tableau_de_mcu);
    free(tab_taille);


    return EXIT_SUCCESS;
}
