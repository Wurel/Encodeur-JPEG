#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "module_bitstream.h"


/*
    Type représentant le flux d'octets à écrire dans le fichier JPEG de
    sortie (appelé bitstream dans le sujet).
*/
struct bitstream
{
  FILE *fichier;
  int8_t *nb_bits;
  uint32_t *valeur;
};


/*
Retourne un nouveau bitstream prêt à écrire dans le fichier filename.
*/
extern struct bitstream *bitstream_create(const char *filename)
{
  struct bitstream *notre_bitstream = malloc(sizeof(struct bitstream));
  FILE* notre_fichier = NULL;
  notre_fichier = fopen(filename , "wb");
  notre_bitstream->fichier = notre_fichier;
  notre_bitstream->nb_bits = calloc(sizeof(int8_t), 1);
  notre_bitstream->valeur = calloc(sizeof(uint32_t), 1);
  return notre_bitstream;
}


/*
    Ecrit nbits bits dans le bitstream. La valeur portée par cet ensemble de
    bits est value. Le paramètre is_marker permet d'indiquer qu'on est en train
    d'écrire un marqueur de section dans l'entête JPEG ou non (voir section
    2.10.4 du sujet): ce is_marker est à 0 quand on est dans l'encodage des données brutes.
*/
void bitstream_write_nbits(struct bitstream *stream, uint32_t value, uint8_t nbits, bool is_marker)
{
  uint8_t a_ecrire = 0;
  a_ecrire = 0;
  *stream -> valeur = (*stream -> valeur << nbits) +value;
  *stream -> nb_bits = *stream -> nb_bits  + nbits;
  while (*stream -> nb_bits >= 8 )
  {
    a_ecrire = *stream -> valeur >> (*stream -> nb_bits -8);
    *stream -> nb_bits = *stream -> nb_bits -8;
    if (is_marker == 0 && a_ecrire == 0xff )
    {

  fputc(a_ecrire, stream -> fichier);
      fputc(0, stream -> fichier);
    }else{
          fputc(a_ecrire, stream -> fichier);
    }
  }
}


/*
    Force l'exécution des écritures en attente sur le bitstream, s'il en
    existe.
*/
extern void bitstream_flush(struct bitstream *stream)
{
  if (*stream -> nb_bits != 0 )
  {
    *stream -> valeur = (*stream -> valeur << (8 - *stream -> nb_bits));
    *stream -> nb_bits = 0;
    fputc(*stream -> valeur, stream -> fichier);
  }
}

/*
    Détruit le bitstream passé en paramètre, en libérant la mémoire qui lui est
    associée.
*/
extern void bitstream_destroy(struct bitstream *stream)
{
  if (*stream -> nb_bits != 0)
  {
    bitstream_flush(stream);
  }
  fclose(stream->fichier);
  free(stream->nb_bits);
  free(stream->valeur);
  free(stream);
}
