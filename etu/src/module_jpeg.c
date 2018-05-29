#include "module_jpeg.h"
#include "bitstream.h"
#include "qtables.h"
#include "htables.h"
#include <stdio.h>
#include <stdlib.h>

struct jpeg_desc *jpeg_desc_create(void){
    struct jpeg_desc *jpeg = malloc(sizeof(struct jpeg_desc));
    jpeg->ppm_filename = malloc(200 * sizeof(char));
    jpeg->jpeg_filename = malloc(200 * sizeof(char));
    jpeg->sampling_factor = malloc(3*sizeof(uint8_t));
    for (uint8_t i = 0; i < 3; i++) {
      jpeg->sampling_factor[i] = malloc(2*sizeof(uint8_t));
    }
    jpeg->qtables = malloc(2*sizeof(uint8_t));
    jpeg->tableau_huffman = malloc(3*sizeof(struct huff_table**));
    for (size_t i = 0; i < 3; i++) {
      jpeg->tableau_huffman[i] = malloc(2*sizeof(struct huff_table*));
    }
    return jpeg;
}

void jpeg_write_header(struct jpeg_desc *jdesc){
    uint8_t N = jdesc->nb_components;
    //Création du bitstream
    jdesc->bits = bitstream_create(jdesc->jpeg_filename);
    //Marqueur debut image jpeg
    bitstream_write_nbits(jdesc->bits, 0xffd8, 16, 1);
    //Marqueur application
    bitstream_write_nbits(jdesc->bits, 0xffe0, 16, 1);
    bitstream_write_nbits(jdesc->bits, 0x0010, 16, 1);
    bitstream_write_nbits(jdesc->bits, 0x4a46, 16, 1);
    bitstream_write_nbits(jdesc->bits, 0x4946, 16, 1);
    bitstream_write_nbits(jdesc->bits, 0x0001, 16, 1);
    bitstream_write_nbits(jdesc->bits, 0x0100, 16, 1);
    bitstream_write_nbits(jdesc->bits, 0x0000, 16, 1);
    bitstream_write_nbits(jdesc->bits, 0x0000, 16, 1);
    bitstream_write_nbits(jdesc->bits, 0x0000, 16, 1);
    //Marqueur Define Quantization table
    bitstream_write_nbits(jdesc->bits, 0xFFDB, 16, 1);
    bitstream_write_nbits(jdesc->bits, 0x0043, 16, 1);
    bitstream_write_nbits(jdesc->bits, 0x00, 8, 1);
    for (uint32_t i = 0; i < 64; i++) {
      bitstream_write_nbits(jdesc->bits, compressed_Y_table[i], 8, 1);
    }
    if (N == 3) {//Si on a une image en couleur
      bitstream_write_nbits(jdesc->bits, 0xFFDB, 16, 1);
      bitstream_write_nbits(jdesc->bits, 0x0043, 16, 1);
      bitstream_write_nbits(jdesc->bits, 0x01, 8, 1);
      for (uint32_t i = 0; i < 64; i++) {
        bitstream_write_nbits(jdesc->bits, compressed_CbCr_table[i], 8, 1);
      }
    }
    //Marqueur Start of Frame
    bitstream_flush(jdesc->bits);
    bitstream_write_nbits(jdesc->bits, 0xFFC0, 16, 1);
      //Taille dépendante du nombre de composante (1 ou 3)
    if (N == 3) {//Si on a une image en couleur, longueur = 11
      bitstream_write_nbits(jdesc->bits, 0x0011, 16, 1);
    }
    else{ //Sinon
      bitstream_write_nbits(jdesc->bits, 0x000B, 16, 1);
    }
    bitstream_write_nbits(jdesc->bits, 0x08, 8, 1); //Précision
    bitstream_write_nbits(jdesc->bits, jpeg_desc_get_image_height(jdesc), 16, 1); //hauteur
    bitstream_write_nbits(jdesc->bits, jpeg_desc_get_image_width(jdesc), 16, 1); //largeur
      //On écrit le nombre de composante N (1 ou 3)
    if (N == 3) {
      bitstream_write_nbits(jdesc->bits, 0x0003, 8, 1);
    }
    else{
      bitstream_write_nbits(jdesc->bits, 0x0001, 8, 1);
    }
      //On écrit N fois
    bitstream_write_nbits(jdesc->bits, 0x01, 8, 1);
    bitstream_write_nbits(jdesc->bits, jdesc->sampling_factor[0][0], 4, 1);
    bitstream_write_nbits(jdesc->bits, jdesc->sampling_factor[0][1], 4, 1);
    bitstream_write_nbits(jdesc->bits, 0, 8, 1);
    if (N == 3) {
      for (size_t i = 1; i < 3; i++) {
        bitstream_write_nbits(jdesc->bits, i+1, 8, 1);
        bitstream_write_nbits(jdesc->bits, jdesc->sampling_factor[i][0], 4, 1);
        bitstream_write_nbits(jdesc->bits, jdesc->sampling_factor[i][1], 4, 1);
        bitstream_write_nbits(jdesc->bits, 1, 8, 1);
      }
    }
    //Marqueur Define Huffman Table
    bitstream_flush(jdesc->bits);
    uint8_t o = 1;
    if (N == 3) {
      o = 2;
    }
    for (uint8_t k = 0; k < 2; k++) {
      for (uint8_t l = 0; l < 1; l++) {
        bitstream_write_nbits(jdesc->bits, 0xFFC4, 16, 1);
        bitstream_write_nbits(jdesc->bits, 3+16 + htables_nb_symbols[k][l], 16, 1);
        bitstream_write_nbits(jdesc->bits, 0, 3, 1);
        bitstream_write_nbits(jdesc->bits, k, 1, 1);
        bitstream_write_nbits(jdesc->bits, 0, 4, 1);
        for (uint8_t i = 0; i < 16; i++) {
          bitstream_write_nbits(jdesc->bits, htables_nb_symb_per_lengths[k][l][i], 8, 1);
        }

        uint8_t compteur = 0;
        for (uint8_t i = 0; i < 16; i++) {
          for (uint8_t j = 0; j < htables_nb_symb_per_lengths[k][l][i]; j++) {
            bitstream_write_nbits(jdesc->bits, htables_symbols[k][l][compteur], 8, 1);
            compteur ++;
          }
        }
      }
    }

    if (N == 3) {
      for (uint8_t k = 0; k < 2; k++) {
        for (uint8_t l = 1; l < 2; l++) {
          bitstream_write_nbits(jdesc->bits, 0xFFC4, 16, 1);
          bitstream_write_nbits(jdesc->bits, 3+16 + htables_nb_symbols[k][l], 16, 1);
          bitstream_write_nbits(jdesc->bits, 0, 3, 1);
          bitstream_write_nbits(jdesc->bits, k, 1, 1);
          bitstream_write_nbits(jdesc->bits, 1, 4, 1);
          for (uint8_t i = 0; i < 16; i++) {
            bitstream_write_nbits(jdesc->bits, htables_nb_symb_per_lengths[k][l][i], 8, 1);
          }

          uint8_t compteur = 0;
          for (uint8_t i = 0; i < 16; i++) {
            for (uint8_t j = 0; j < htables_nb_symb_per_lengths[k][l][i]; j++) {
              bitstream_write_nbits(jdesc->bits, htables_symbols[k][l][compteur], 8, 1);
              compteur ++;
            }
          }
        }
      }
    }

    bitstream_flush(jdesc->bits);

    if (N == 1) {
      bitstream_write_nbits(jdesc->bits, 0xffda, 16, 1);
      bitstream_write_nbits(jdesc->bits, 0x08, 16, 1);
      bitstream_write_nbits(jdesc->bits, 0x01, 8, 1);
      bitstream_write_nbits(jdesc->bits, 0x0100, 16, 1);
      bitstream_write_nbits(jdesc->bits, 0x0, 8,1);
      bitstream_write_nbits(jdesc->bits, 63, 8,1);
      bitstream_write_nbits(jdesc->bits, 0x0, 8,1);
    }
    else{
      bitstream_write_nbits(jdesc->bits, 0xffda, 16, 1);
      bitstream_write_nbits(jdesc->bits, 0x0c, 16, 1);
      bitstream_write_nbits(jdesc->bits, 0x03, 8, 1);
      bitstream_write_nbits(jdesc->bits, 0x0100, 16, 1);
      bitstream_write_nbits(jdesc->bits, 0x0211, 16, 1);
      bitstream_write_nbits(jdesc->bits, 0x0311, 16, 1);
      bitstream_write_nbits(jdesc->bits, 0, 8, 1);
      bitstream_write_nbits(jdesc->bits, 0x3f, 8, 1);
      bitstream_write_nbits(jdesc->bits, 0, 8, 1);
    }
    bitstream_flush(jdesc->bits);

}

void jpeg_write_footer(struct jpeg_desc *jdesc){
  printf("coucou bande de nouilles\n");
  bitstream_flush(jdesc->bits);
  bitstream_write_nbits(jdesc->bits, 0xFFD9, 16, 1);
}

void jpeg_desc_destroy(struct jpeg_desc *jdesc){
  free(jdesc->ppm_filename);
  free(jdesc->jpeg_filename);
  for (uint8_t i = 0; i < 3; i++) {
    free(jdesc->sampling_factor[i]);
    free(jdesc->tableau_huffman[i]);
  }
  free(jdesc->sampling_factor);
  free(jdesc->tableau_huffman);
  free(jdesc->qtables);
}

void jpeg_desc_set_ppm_filename(struct jpeg_desc *jdesc, const char *ppm_filename){
  strcpy(jdesc->ppm_filename, ppm_filename);
}

char *jpeg_desc_get_ppm_filename(struct jpeg_desc *jdesc){
  return jdesc->ppm_filename;
}

void jpeg_desc_set_jpeg_filename(struct jpeg_desc *jdesc, const char *jpeg_filename){
  strcpy(jdesc->jpeg_filename, jpeg_filename);
}

char *jpeg_desc_get_jpeg_filename(struct jpeg_desc *jdesc){
  return jdesc->ppm_filename;
}

void jpeg_desc_set_image_height(struct jpeg_desc *jdesc, uint32_t image_height){
  jdesc->image_height = image_height;
}

uint32_t jpeg_desc_get_image_height(struct jpeg_desc *jdesc){
  return jdesc->image_height;
}

void jpeg_desc_set_image_width(struct jpeg_desc *jdesc, uint32_t image_width){
  jdesc->image_width = image_width;
}

uint32_t jpeg_desc_get_image_width(struct jpeg_desc *jdesc){
  return jdesc->image_width;
}

void jpeg_desc_set_nb_components(struct jpeg_desc *jdesc, uint8_t nb_components){
  jdesc->nb_components = nb_components;
}

uint8_t jpeg_desc_get_nb_components(struct jpeg_desc *jdesc){
  return jdesc->nb_components;
}

struct bitstream *jpeg_desc_get_bitstream(struct jpeg_desc *jdesc){
  return jdesc->bits;
}

void jpeg_desc_set_sampling_factor(struct jpeg_desc *jdesc, enum color_component cc, enum direction dir, uint8_t sampling_factor){
  jdesc->sampling_factor[cc][dir] = sampling_factor;
}

uint8_t jpeg_desc_get_sampling_factor(struct jpeg_desc *jdesc, enum color_component cc, enum direction dir){
  return jdesc->sampling_factor[cc][dir];
}

void jpeg_desc_set_huffman_table(struct jpeg_desc *jdesc, enum sample_type acdc, enum color_component cc, struct huff_table *htable){
  jdesc->tableau_huffman[cc][acdc] = htable;
}

struct huff_table *jpeg_desc_get_huffman_table(struct jpeg_desc *jdesc, enum sample_type acdc, enum color_component cc){
  return jdesc->tableau_huffman[cc][acdc];
}

void jpeg_desc_set_quantization_table(struct jpeg_desc *jdesc, enum color_component cc, uint8_t *qtable){
  jdesc->qtables[cc] = *qtable;
}

uint8_t *jpeg_desc_get_quantization_table(struct jpeg_desc *jdesc, enum color_component cc){
  return &jdesc->qtables[cc];
}
