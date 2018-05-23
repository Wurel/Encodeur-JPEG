// #ifndef module_jpeg
// #define module_jpeg
// #include <stdint.h>
// #include <string.h>
// #include <stdlib.h>
// #include "huffman.h"
//
// struct jpeg_desc_eleve{
//   struct bitstream *bits;
//   char *ppm_filename;
//   char *jpeg_filename;
//   uint32_t image_height;
//   uint32_t image_width;
//   uint8_t nb_components;
//   uint8_t **sampling_factor; //3*2
//   struct huff_table ***tableau_huffman; //3][2]
//   uint8_t *qtables; //3
// };
//
// // /* Type énuméré représentant les composantes de couleur YCbCr. */
// // enum color_component
// // {
// //     Y,
// //     Cb,
// //     Cr,
// //     NB_COLOR_COMPONENTS
// // };
// //
// // /*
// //     Type énuméré représentant les types de composantes fréquentielles (DC ou
// //     AC).
// // */
// // enum sample_type
// // {
// //     DC,
// //     AC,
// //     NB_SAMPLE_TYPES
// // };
// //
// // /*
// //     Type énuméré représentant la direction des facteurs d'échantillonnage (H
// //     pour horizontal, V pour vertical).
// // */
// // enum direction
// // {
// //     H,
// //     V,
// //     NB_DIRECTIONS
// // };
//
// /* Ecrit le nom de fichier PPM ppm_filename dans le jpeg_desc_eleve jdesc. */
// extern struct jpeg_desc_eleve *jpeg_desc_eleve_create_eleve(void);
//
// /*
//     Ecrit toute l'entête JPEG dans le fichier de sortie à partir des
//     informations contenues dans le jpeg_desc passé en paramètre. En sortie, le
//     bitstream est positionné juste après l'écriture de l'entête SOS, à
//     l'emplacement du premier octet de données brutes à écrire.
// */
// extern void jpeg_desc_write_header_eleve(struct jpeg_desc_eleve *jdesc);
//
// /* Ecrit le nom du fichier de sortie jpeg_filename dans le jpeg_desc_eleve jdesc. */
// extern void jpeg_desc_eleve_set_ppm_filename_eleve(struct jpeg_desc_eleve *jdesc,
//                                        const char *ppm_filename);
//
// /* Retourne le nom de fichier PPM lu dans le jpeg_desc_eleve jdesc. */
// extern char *jpeg_desc_eleve_get_ppm_filename_eleve(struct jpeg_desc_eleve *jdesc);
//
// /* Ecrit le nom du fichier de sortie jpeg_filename dans le jpeg_desc_eleve jdesc. */
// extern void jpeg_desc_eleve_set_jpeg_filename_eleve(struct jpeg_desc_eleve *jdesc, const char *jpeg_filename);
//
// /* Retourne le nom du fichier de sortie lu depuis le jpeg_desc_eleve jdesc. */
// extern char *jpeg_desc_eleve_get_jpeg_filename_eleve(struct jpeg_desc_eleve *jdesc);
//
// /*
//     Ecrit la hauteur en nombre de pixels de l'image traitée image_height dans
//     le jpeg_desc_eleve jdesc.
// */
// extern void jpeg_desc_eleve_set_image_height_eleve(struct jpeg_desc_eleve *jdesc, uint32_t image_height);
// /*
//     Retourne la hauteur en nombre de pixels de l'image traitée lue dans le
//     jpeg_desc_eleve jdesc.
// */
// extern uint32_t jpeg_desc_eleve_get_image_height_eleve(struct jpeg_desc_eleve *jdesc);
//
// /*
//     Ecrit la largeur en nombre de pixels de l'image traitée image_width dans le
//     jpeg_desc_eleve jdesc.
// */
// extern void jpeg_desc_eleve_set_image_width_eleve(struct jpeg_desc_eleve *jdesc, uint32_t image_width);
//
// /*
//     Retourne la largeur en nombre de pixels de l'image traitée lue dans le
//     jpeg_desc_eleve jdesc.
// */
// extern uint32_t jpeg_desc_eleve_get_image_width_eleve(struct jpeg_desc_eleve *jdesc);
//
// /*
//     Ecrit le nombre de composantes de couleur de l'image traitée nb_components
//     dans le jpeg_desc_eleve jdesc.
// */
// extern void jpeg_desc_eleve_set_nb_components_eleve(struct jpeg_desc_eleve *jdesc, uint8_t nb_components);
//
// /*
//     Retourne le nombre de composantes de couleur de l'image traitée lu à partir
//     du jpeg_desc_eleve jdesc.
// */
// extern uint8_t jpeg_desc_eleve_get_nb_components_eleve(struct jpeg_desc_eleve *jdesc);
//
// /*
//     Retourne le bitstream associé au fichier de sortie enregistré dans le
//     jpeg_desc_eleve.
// */
// extern struct bitstream *jpeg_desc_eleve_get_bitstream_eleve(struct jpeg_desc_eleve *jdesc);
//
// /*
//     Ecrit dans le jpeg_desc_eleve jdesc le facteur d'échantillonnage sampling_factor
//     à utiliser pour la composante de couleur cc et la direction dir.
// */
// extern void jpeg_desc_eleve_set_sampling_factor_eleve(struct jpeg_desc_eleve *jdesc,
//                                           enum color_component cc,
//                                           enum direction dir,
//                                           uint8_t sampling_factor);
//
// /*
//     Retourne le facteur d'échantillonnage utilisé pour la composante de couleur
//     cc et la direction dir, lu à partir du jpeg_desc_eleve jdesc.
// */
// extern uint8_t jpeg_desc_eleve_get_sampling_factor_eleve(struct jpeg_desc_eleve *jdesc,
//                                              enum color_component cc,
//                                              enum direction dir);
//
// /*
// Ecrit dans le jpeg_desc_eleve jdesc la table de Huffman huff_table à utiliser
// pour encoder les données de la composante fréquentielle acdc pour la
// composante de couleur cc.
// */
// extern void jpeg_desc_eleve_set_huffman_table_eleve(struct jpeg_desc_eleve *jdesc, enum sample_type acdc, enum color_component cc, struct huff_table *htable);
//
// /*
//     Retourne un pointeur vers la table de Huffman utilisée pour encoder les
//     données de la composante fréquentielle acdc pour la composante de couleur
//     cc, lue à partir du jpeg_desc_eleve jdesc.
// */
// extern struct huff_table *jpeg_desc_eleve_get_huffman_table_eleve(struct jpeg_desc_eleve *jdesc, enum sample_type acdc, enum color_component cc);
//
// /*
//     Ecrit dans le jpeg_desc_eleve jdesc la table de quantification qtable à utiliser
//     pour compresser les coefficients de la composante de couleur cc.
// */
// extern void jpeg_desc_eleve_set_quantization_table_eleve(struct jpeg_desc_eleve *jdesc, enum color_component cc, uint8_t *qtable);
//
// /*
//     Retourne la table de quantification associée à la composante de couleur
//     cc dans le jpeg_desc_eleve jdesc.
// */
// extern uint8_t *jpeg_desc_eleve_get_quantization_table_eleve(struct jpeg_desc_eleve *jdesc,
//                                                  enum color_component cc);
//
//   #endif /* module_jpeg */
