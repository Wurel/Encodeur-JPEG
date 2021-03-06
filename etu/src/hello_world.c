#include <stdlib.h>
#include <stdio.h>
#include "decoupage_mcu.h"
#include "dct.h"
#include "rgb2ycbcr.h"
#include "q_zz.h"
#include "AC_DC.h"
#include "htables.h"
#include "qtables.h"
#include "module_bitstream.h" //Le notre !
#include "huffman_eleve.h"
#include <unistd.h>
#include "recuperation.h"
#include "down_sampler.h"
#include "module_jpeg.h"
#include <time.h>

int main(int argc, char const *argv[])
{
    uint8_t sample[6] = {1,1,1,1,1,1};
    clock_t t1, t2;
    t1 = clock();
    printf("Au boulot!\n\n");
    char sortie[200] = "";
    char entree[200];
    recuperation_argument(argc, argv, entree, sortie, sample);
    uint8_t type_couleur = type(entree); //type_couleur = 1 si Noir et Blanc et type_couleur = 3 pour couleur
    uint8_t h1 = sample[0];
    uint8_t v1 = sample[1];
    uint8_t h2 = sample[2];
    uint8_t v2 = sample[3];
    uint8_t h3 = sample[4];
    uint8_t v3 = sample[5];
    if (type_couleur == 1 && (h1 != 1 || v1 != 1 || h2 != 1 || v2 != 1 || h3 != 1 || v3 != 1))
    {
        printf("Image en nuance de gris, le programme continue avec des facteurs d'échantillonnages égaux à 1...\n");
        h1 = 1;
        h2 = 1;
        h3 = 1;
        v1 = 1;
        v2 = 1;
        v3 = 1;
    }
    if (h1+h2+h3+v1+v2+v3 > 10)
    {
        printf("Somme des facteurs d'échantillonnages trop grande ... Sortie du programme :'(\n");
        exit(1);
    }
    if (h2 > h1 || h3 > h1 || v2 > v1 || v3 > v1 || h2 != h3 || v2 != v3)
    {
        printf("Facteurs d'échantillonnages incohérents ... Sortie du programme :'( \n");
        exit(1);
    }
    if ((h1 != 1 && h1 != 2 && h1 != 4) || (v1 != 1 && v1 != 2 && v1 != 4))
    {
        printf("Valeurs de h1xv1 impossible ... Sortie du programme\n");
        exit(1);
    }
    struct mcu **tableau_de_mcu;
    uint32_t *tab_taille = NULL;
    tab_taille = taille_tableau(entree,1,1);
    uint32_t *tab_taille_x8 = NULL;
    tab_taille_x8 = taille_tableau_x8(entree);
    uint32_t largeur = tab_taille_x8[0];
    uint32_t hauteur = tab_taille_x8[1];
    free(tab_taille_x8);
    // Découpage
    tableau_de_mcu = decoupage_mc(entree,h1,v1);
    printf("Recuperation RGB faite et decoupage mcu\n");
    // RGB -> YCbCr, DCT
    //PARCOURS DES MCUS
    for (uint32_t i = 0; i < ajustement_taille(hauteur, v1)/(8*v1); i++)
    {
      for (uint32_t j = 0; j < ajustement_taille(largeur, h1)/(8*h1); j++)
      {
        //PARCOURS DES BLOCS
        // tableau_de_mcu[i][j].tableau_de_bloc_apres_dct = malloc(h1*v1*sizeof(struct bloc_apres_dct));
        for (size_t k = 0; k < h1*v1; k++)
        {
          //rgb 2 ycbcr
          transformation_bloc_rgb_ycbcr(&tableau_de_mcu[i][j].tableau_de_bloc[k]);
        }
        //DOWN SAMPLER
        uint8_t facteur_horizontal = 1;
        if (type_couleur == 3)
        {
            if(h2<h1)
            {
                echantillonnage_horizontal(tableau_de_mcu[i][j], h1/h2);
                facteur_horizontal = h1/h2;
            }
            if (v2<v1)
            {
                echantillonnage_vertical(tableau_de_mcu[i][j], v1/v2, facteur_horizontal);
            }
        }
        for(size_t k=0; k<h1*v1; k++)
        {
          // DCT
          tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].y = malloc(sizeof(int16_t)*64);
          dct_bloc(tableau_de_mcu[i][j].tableau_de_bloc[k].y, tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].y);

          if(k%(h1/h2)==0 && (k/h1)%(v1/v2)==0)
          {
            tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cb = malloc(sizeof(int16_t)*64);
            dct_bloc(tableau_de_mcu[i][j].tableau_de_bloc[k].cb, tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cb);

            tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cr = malloc(sizeof(int16_t)*64);
            dct_bloc(tableau_de_mcu[i][j].tableau_de_bloc[k].cr, tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cr);
            tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cb =
            zigzag_composante(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cb);

            tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cr =
            zigzag_composante(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cr);
            quantification_Cb_Cr(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cb);
            quantification_Cb_Cr(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cr);
          }
          // // //Zigzag
          tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].y =
                                  zigzag_composante(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].y);

          //
          // //Quantification
          //
          quantification_Y(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].y);
          // si on a besoin de toutes les mcus
          // printf("mcu : [%d, %d]\n", i,j);
          // for (size_t p = 0; p < 8; p++) {
          //   for (size_t h = 0; h < 8; h++) {
          //     printf("%hx\t", tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].y[p*8+h]);
          //   }
          //   printf("\n");
          // }
          // printf("nouvelle mcu\n");
        }
      }
    }
    printf("Transformation des blocs terminee\n");
    //Si on a besoin d'afficher une mci
    // for (uint32_t i = 0; i < 8; i++) {
    //   for (uint32_t j = 0; j < 8; j++) {
    //     printf("%x\t", tableau_de_mcu[0][0].tableau_de_bloc_apres_dct[0].y[j+i*8]);
    //   }
    //   printf("\n");
    // }
    struct jpeg_desc *jpeg = jpeg_desc_create();
    jpeg_desc_set_ppm_filename(jpeg, entree);
    jpeg_desc_set_jpeg_filename(jpeg, sortie);
    jpeg_desc_set_nb_components(jpeg, type_couleur);
    jpeg_desc_set_image_width(jpeg, largeur);
    jpeg_desc_set_image_height(jpeg, hauteur);

    jpeg_desc_set_sampling_factor(jpeg, Y, H, h1);
    jpeg_desc_set_sampling_factor(jpeg, Y, V, v1);
    struct huff_table *mon_arbre = huffman_table_build(htables_nb_symb_per_lengths[0][0],
                          htables_symbols[0][0]);
    struct huff_table *mon_arbre_2 = huffman_table_build(htables_nb_symb_per_lengths[0][1], htables_symbols[0][1]);
    struct huff_table *mon_arbre_3 = huffman_table_build(htables_nb_symb_per_lengths[1][1], htables_symbols[1][1]);
    struct huff_table *mon_arbre_4 = huffman_table_build(htables_nb_symb_per_lengths[0][2], htables_symbols[0][2]);
    struct huff_table *mon_arbre_5 = huffman_table_build(htables_nb_symb_per_lengths[1][2], htables_symbols[1][2]);

    jpeg_desc_set_huffman_table(jpeg, DC, Y, mon_arbre);
    struct huff_table *mon_arbre_1 = huffman_table_build(htables_nb_symb_per_lengths[1][0], htables_symbols[1][0]);
    jpeg_desc_set_huffman_table(jpeg, AC, Y, mon_arbre_1);
    jpeg_desc_set_quantization_table(jpeg, Y, compressed_Y_table);
    if (type_couleur == 3) {
      //couleur
      jpeg_desc_set_sampling_factor(jpeg, Cb, H, h2);
      jpeg_desc_set_sampling_factor(jpeg, Cb, V, v2);
      jpeg_desc_set_huffman_table(jpeg, DC, Cb, mon_arbre_2);
      jpeg_desc_set_huffman_table(jpeg, AC, Cb, mon_arbre_3);
      jpeg_desc_set_quantization_table(jpeg, Cb, compressed_CbCr_table);

      jpeg_desc_set_sampling_factor(jpeg, Cr, H, h3);
      jpeg_desc_set_sampling_factor(jpeg, Cr, V, v3);
      jpeg_desc_set_huffman_table(jpeg, DC, Cr, mon_arbre_4);
      jpeg_desc_set_huffman_table(jpeg, AC, Cr, mon_arbre_5);
      jpeg_desc_set_quantization_table(jpeg, Cr, compressed_CbCr_table);
    }
    jpeg_write_header(jpeg);
    printf("Ecriture de l'entete jpeg terminee\n");
    struct bitstream *bits; //= bitstream_create("/user/6/.base/poraa/home/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/test.jpeg");
    bits = jpeg_desc_get_bitstream(jpeg);
    ecriture_AC_DC_complete(bits, tableau_de_mcu, ajustement_taille(largeur, h1)/8/h1, ajustement_taille(hauteur, v1)/8/v1, h1, v1, h2, v2, h3, v3, type_couleur, jpeg);
    printf("Ecriture des donnes brutes terminee\n");
    jpeg_write_footer(jpeg);
    jpeg_desc_destroy(jpeg);


    for (uint32_t i = 0; i < ajustement_taille(hauteur, v1)/(8*v1); i++)
    {
      for (uint32_t j = 0; j < ajustement_taille(largeur, h1)/(8*h1); j++)
      {
        for (size_t k = 0; k < h1*v1; k++)
        {
          free(tableau_de_mcu[i][j].tableau_de_bloc[k].rgb);
          free(tableau_de_mcu[i][j].tableau_de_bloc[k].y);
          free(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].y);

        }
        for (size_t k = 0; k < h1*v1; k=k+(h1/h2)) {
          if (tableau_de_mcu[i][j].tableau_de_bloc[k].cb != NULL)
          {
            free(tableau_de_mcu[i][j].tableau_de_bloc[k].cb);
            tableau_de_mcu[i][j].tableau_de_bloc[k].cb = NULL;
          }
          if (tableau_de_mcu[i][j].tableau_de_bloc[k].cr != NULL)
          {
            free(tableau_de_mcu[i][j].tableau_de_bloc[k].cr);
            tableau_de_mcu[i][j].tableau_de_bloc[k].cr = NULL;
          }
          if (tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cb != NULL)
          {
            free(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cb);
            tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cb = NULL;
          }
          if (tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cr != NULL)
          {
            free(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cr);
            tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cr = NULL;
          }
          if(v2<v1)
          {
            if((k+(h1/h2))%h1 == 0)
            {
              k+=h1*v1/v2-1;
            }
          }
        }
        free(tableau_de_mcu[i][j].tableau_de_bloc);
        free(tableau_de_mcu[i][j].tableau_de_bloc_apres_dct);
      }
    }
    for (uint32_t i = 0; i <ajustement_taille(hauteur, v1)/(8*v1); i++)
    {
      free(tableau_de_mcu[i]);
    }
   free(tableau_de_mcu);
   free(tab_taille);
   huffman_table_destroy(mon_arbre);
   huffman_table_destroy(mon_arbre_1);
   huffman_table_destroy(mon_arbre_2);
   huffman_table_destroy(mon_arbre_3);
   huffman_table_destroy(mon_arbre_4);
   huffman_table_destroy(mon_arbre_5);
   printf("Frees termines\n");
   t2 = clock();
   float temps = (float)(t2-t1)/CLOCKS_PER_SEC;
   printf("temps = %f\n", temps);

   return EXIT_SUCCESS;
}
