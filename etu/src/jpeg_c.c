// #include "huffman.h"
// #include "htables.h"
// #include "qtables.h"
// #include "bitstream.h"
// #include "jpeg_c.h"
// #include "decoupage_mcu.h"
// #include "structures.h"
// #include "AC_DC.h"
//
// void all_set(struct jpeg_desc *image, struct mcu **tableau_de_mcu, char nom, uint32_t tipe, uint32_t hauteur, uint32_t largeur) {
//   jpeg_desc_set_ppm_filename(image, &nom);
//   jpeg_desc_set_jpeg_filename(image, strcat(&nom, "jpeg"));
//   jpeg_desc_set_nb_components(image, tipe);
//   jpeg_desc_set_image_width(image, largeur); // atttention h1 V1
//   jpeg_desc_set_image_height(image, hauteur);
//
//   jpeg_desc_set_sampling_factor(image, Y, H, 1);
//   jpeg_desc_set_sampling_factor(image, Y, V, 1);
//   struct huff_table *mon_arbre;
//   for (uint32_t i = 0; i < tipe; i++) {
//     for (uint32_t j = 0; j < 2; j++) {
//       mon_arbre = huffman_table_build(htables_nb_symb_per_lengths[j][i], htables_symbols[j][i], htables_nb_symbols[j][i]);
//       jpeg_desc_set_huffman_table(image, j, i, mon_arbre);
//     }
//   }
//   huffman_table_destroy(mon_arbre);
//
//   if (tipe == 3) {
//     jpeg_desc_set_sampling_factor(image, Cb, H, 1);
//     jpeg_desc_set_sampling_factor(image, Cb, V, 1);
//     jpeg_desc_set_quantization_table(image, Cb, compressed_CbCr_table);
//     jpeg_desc_set_sampling_factor(image, Cr, H, 1);
//     jpeg_desc_set_sampling_factor(image, Cr, V, 1);
//     jpeg_desc_set_quantization_table(image, Cr, compressed_CbCr_table);
//   }
//   jpeg_write_header(image);
//   struct bitstream *bits; //= bitstream_create("/user/6/.base/poraa/home/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/test.jpeg");
//   bits = jpeg_desc_get_bitstream(image);
//   ecriture_AC_DC_complete(bits, tableau_de_mcu, ajustement_taille(hauteur)/8, ajustement_taille(largeur)/8, tipe);
//   jpeg_write_footer(image);
// }
