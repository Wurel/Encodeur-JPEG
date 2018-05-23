// #include "module_jpeg.h"
// #include "bitstream.h"
//
// struct jpeg_desc_eleve *jpeg_desc_eleve_create_eleve(void){
//     struct jpeg_desc_eleve *jpeg = malloc(sizeof(struct jpeg_desc_eleve));
//     jpeg->ppm_filename = malloc(200 * sizeof(char));
//     jpeg->jpeg_filename = malloc(200 * sizeof(char));
//     jpeg->sampling_factor = malloc(3*sizeof(uint8_t));
//     for (uint8_t i = 0; i < 3; i++) {
//       jpeg->sampling_factor[i] = malloc(2*sizeof(uint8_t));
//     }
//     jpeg->qtables = malloc(2*sizeof(uint8_t));
//     jpeg->tableau_huffman = malloc(3*sizeof(struct huff_table**));
//     for (size_t i = 0; i < 3; i++) {
//       jpeg->tableau_huffman[i] = malloc(2*sizeof(struct huff_table*));
//     }
//     return jpeg;
// }
//
// void jpeg_desc_write_header_eleve(struct jpeg_desc_eleve *jdesc){
//     //Création du bitstream
//     jpeg_desc->bits = bitstream_create(jdesc->jpeg_filename);
//     //Marqueur debut image jpeg
//     bitstream_write_nbits(jpeg_desc->bits, 0xffd8, 16, 1);
//     //Marqueur application
//     bitstream_write_nbits(jdesc->bits, 0xffe0, 16, 1);
//     bitstream_write_nbits(jdesc->bits, 0x0010, 16, 0);
//     bitstream_write_nbits(jdesc->bits, 0x4a46, 16, 0);
//     bitstream_write_nbits(jdesc->bits, 0x4649, 16, 0);
//     bitstream_write_nbits(jdesc->bits, 0x0001, 16, 0);
//     bitstream_write_nbits(jdesc->bits, 0x0100, 16, 0);
//     bitstream_write_nbits(jdesc->bits, 0x0000, 16, 0);
//     bitstream_write_nbits(jdesc->bits, 0x0000, 16, 0);
//     bitstream_write_nbits(jdesc->bits, 0x0000, 16, 0);
//     //Marqueur Define Quantization table
//     bitstream_write_nbits(jdesc->bits, 0xFFDB, 16, 1);
//     bitstream_write_nbits(jdesc->bits, 0x0043, 16, 0);
//     bitstream_write_nbits(jdesc->bits, 0x00, 8, 0);
//     for (uint32_t i = 0; i < 64; i++) {
//       bitstream_write_nbits(jdesc->bits, compressed_Y_table[i], 8, 0);
//     }
//     if (jpeg_desc_eleve_get_nb_components_eleve(jdesc) == 3) {//Si on a une image en couleur
//       bitstream_write_nbits(jdesc->bits, 0x01, 8, 0);
//       for (uint32_t i = 0; i < 64; i++) {
//         bitstream_write_nbits(jdesc->bits, compressed_CbCr_table[i], 8, 0);
//       }
//     }
//     //Marqueur Start of Frame
//     bitstream_write_nbits(jdesc->bits, 0xFFC0, 16, 1);
//       //Taille dépendante du nombre de composante (1 ou 3)
//     if (jpeg_desc_eleve_get_nb_components_eleve(jpeg_desc) == 3) {//Si on a une image en couleur
//       bitstream_write_nbits(jdesc->bits, 0x0011, 16, 0);
//     }
//     else{ //Sinon
//       bitstream_write_nbits(jdesc->bits, 0x000B, 16, 0);
//     }
//     bitstream_write_nbits(jdesc->bits, 0x08, 8, 0);
//     bitstream_write_nbits(jdesc->bits, jpeg_desc_eleve_get_image_height_eleve(jpeg_desc), 16, 0)
//     bitstream_write_nbits(jdesc->bits, jpeg_desc_eleve_get_image_width_eleve(jpeg_desc), 16, 0)
//       //On écrit le nombre de composante N (1 ou 3)
//     if (jpeg_desc_eleve_get_nb_components_eleve(jdesc) == 3) {
//       bitstream_write_nbits(jdesc->bits, 0x0003, 8, 0);
//     }
//     else{
//       bitstream_write_nbits(jdesc->bits, 0x0001, 8, 0);
//     }
//       //On écrit N fois
//     bitstream_write_nbits(jdesc->bits, 0x01, 8, 0);
//     bitstream_write_nbits(jdesc->bits, jdesc->sampling_factor[0][0], 4, 0);
//     bitstream_write_nbits(jdesc->bits, jdesc->sampling_factor[0][1], 4, 0);
//     bitstream_write_nbits(jdesc->bits, 0, 8, 0);
//     if (jpeg_desc_eleve_get_nb_components_eleve(jdesc) == 3) {
//       for (size_t i = 1; i < 3; i++) {
//         bitstream_write_nbits(jdesc->bits, i+1, 1, 0)
//         bitstream_write_nbits(jdesc->bits, jdesc->sampling_factor[i][0], 4, 0);
//         bitstream_write_nbits(jdesc->bits, jdesc->sampling_factor[i][1], 4, 0);
//         bitstream_write_nbits(jdesc->bits, 1, 8, 0);
//       }
//     }
//     //Marqueur Define Huffman Table
//     bitstream_write_nbits(jdesc->bits, 0xFFC4, 8, 1);
//     //bitstream_write_nbits(jpeg_desc->bits, longueur, 8, 0);
//     bitstream_write_nbits(jdesc->bits, 0x0, 3, 0);
//     bitstream_write_nbits(jdesc->bits, 0x0, 1, 0);
//     bitstream_write_nbits(jdesc->bits, 0, 4, 0);
//     //bitstream_write_nbits(jpeg_desc->bits, )
// }
//
//
// void jpeg_desc_eleve_set_ppm_filename_eleve(struct jpeg_desc_eleve *jdesc, const char *ppm_filename){
//   strcpy(jdesc->ppm_filename, ppm_filename);
// }
//
// char *jpeg_desc_eleve_get_ppm_filename_eleve(struct jpeg_desc_eleve *jdesc){
//   return jdesc->ppm_filename;
// }
//
// void jpeg_desc_eleve_set_jpeg_filename_eleve(struct jpeg_desc_eleve *jdesc, const char *jpeg_filename){
//   strcpy(jdesc->jpeg_filename, jpeg_filename);
// }
//
// char *jpeg_desc_eleve_get_jpeg_filename_eleve(struct jpeg_desc_eleve *jdesc){
//   return jdesc->ppm_filename;
// }
//
// void jpeg_desc_eleve_set_image_height_eleve(struct jpeg_desc_eleve *jdesc, uint32_t image_height){
//   jdesc->image_height = image_height;
// }
//
// uint32_t jpeg_desc_eleve_get_image_height_eleve(struct jpeg_desc_eleve *jdesc){
//   return jdesc->image_height;
// }
//
// void jpeg_desc_eleve_set_image_width_eleve_eleve(struct jpeg_desc_eleve *jdesc, uint32_t image_width){
//   jdesc->image_width = image_width;
// }
//
// uint32_t jpeg_desc_eleve_get_image_width_eleve(struct jpeg_desc_eleve *jdesc){
//   return jdesc->image_width;
// }
//
// void jpeg_desc_eleve_set_nb_components_eleve(struct jpeg_desc_eleve *jdesc, uint8_t nb_components){
//   jdesc->nb_components = nb_components;
// }
//
// uint8_t jpeg_desc_eleve_get_nb_components_eleve(struct jpeg_desc_eleve *jdesc){
//   return jdesc->nb_components;
// }
//
// struct bitstream *jpeg_desc_eleve_get_bitstream_eleve(struct jpeg_desc_eleve *jdesc){
//   return jdesc->bits;
// }.0
//
// void jpeg_desc_eleve_set_sampling_factor_eleve(struct jpeg_desc_eleve *jdesc, enum color_component cc, enum direction dir, uint8_t sampling_factor){
//   jdesc->sampling_factor[cc][dir] = sampling_factor;
// }
//
// uint8_t jpeg_desc_eleve_get_sampling_factor_eleve(struct jpeg_desc_eleve *jdesc, enum color_component cc, enum direction dir){
//   return jdesc->sampling_factor[cc][dir];
// }
//
// void jpeg_desc_eleve_set_huffman_table_eleve(struct jpeg_desc_eleve *jdesc, enum sample_type acdc, enum color_component cc, struct huff_table *htable){
//   jdesc->tableau_huffman[cc][acdc] = htable;
// }
//
// struct huff_table *jpeg_desc_eleve_get_huffman_table_eleve(struct jpeg_desc_eleve *jdesc, enum sample_type acdc, enum color_component cc){
//   return jdesc->tableau_huffman[cc][acdc];
// }
//
// void jpeg_desc_eleve_set_quantization_table_eleve(struct jpeg_desc_eleve *jdesc, enum color_component cc, uint8_t *qtable){
//   jdesc->qtables[cc] = *qtable;
// }
//
// uint8_t *jpeg_desc_eleve_get_quantization_table_eleve(struct jpeg_desc_eleve *jdesc, enum color_component cc){
//   return &jdesc->qtables[cc];
// }
