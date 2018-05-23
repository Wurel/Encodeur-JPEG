// #include <stdint.h>
// #include <stdbool.h>
// #include "module_bitstream.h"
//
// struct bitstream_eleve
// {
//   FILE *fichier;
//   int8_t nb_bits;
//   uint32_t valeur;
// };
//
//
// struct bitstream_eleve bitstream_create_eleve(const char *filename){
//   struct bitstream_eleve bite = {fopen(filename), 0, 0};
//   return bite;
// }
//
// void bitstream_write_nbits_eleve(struct bitstream_eleve *stream, uint32_t value, uint8_t nbits, bool is_marker)
// {
//   uint8_t reste = 0;
//   while (stream -> nb_bits + nbits > 8 ) {
//     reste = 8 - stream->nb_bits;
//     stream -> valeur = (stream -> valeur << reste) + (value >> (nbits - reste));
//     stream -> nb_bits = 0;
//     nbits = nbits - reste;
//     value = value - ((value >> (nbits -reste)) << nbits);
//     fputc(stream -> valeur, stream -> fichier);
//     stream -> valeur = 0;
//   }
//   if (stream->nb_bits + nbits < 8 ) {
//     stream->nb_bits += nbits;
//     stream->valeur = (stream->valeur << nbits) + value;
//   }
//   else if (stream -> nb_bits + nbits == 8) {
//     stream -> nb_bits = 0;
//     stream->valeur = (stream->valeur << nbits) + value;
//     fputc(stream -> valeur, stream -> fichier);
//     stream -> valeur = 0;
//   }
// }
