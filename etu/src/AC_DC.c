#include "AC_DC.h"
#include <math.h>
#include "bitstream.h"
#include <stdlib.h>
#include "huffman.h"
#include "htables.h"

uint8_t retourne_magnitude(int16_t nombre)
{
  for (uint8_t m = 0; m < 12; m++) {
    if ((-(pow(2,m)-1) <= nombre && nombre <= -pow(2,m-1)) || (pow(2,m-1) <= nombre && nombre <= pow(2,m)-1))
    {
      return m;
    }
  }
  exit(0);
}

int16_t retourne_bits(int16_t nombre, uint8_t magnitude)
{
  if (nombre < 0)
  {
    int16_t retour = nombre + pow(2, magnitude) - 1;
    return retour;
  }
  else
  {
    return nombre;
  }
}



void ecriture_symbole_DC(struct bitstream *stream, uint16_t nombre)
{
  uint32_t symbole_decode;
  struct huff_table *mon_arbre = huffman_table_build(htables_nb_symb_per_lengths[0][0],
                      htables_symbols[0][0],
                      htables_nb_symbols[0][0]);
  uint8_t magnitude = retourne_magnitude(nombre);
  uint8_t *nbits = malloc(sizeof(uint8_t));
  symbole_decode = huffman_table_get_path(mon_arbre, magnitude, nbits);
  printf("hexa %x\n", symbole_decode);
  bitstream_write_nbits(stream, symbole_decode, *nbits, 0);
  free(nbits);
  int16_t bits = retourne_bits(nombre, magnitude);
  printf("nombre %x\n", bits);
  // uint8_t *nbits2 = malloc(sizeof(uint8_t));
  // printf("ici ici %x\n", bits);
  // symbole_decode = huffman_table_get_path(mon_arbre, bits, nbits2);
  bitstream_write_nbits(stream, bits, magnitude, 0);
}


void ecriture_symbole_AC(struct bitstream *stream, uint32_t symbole_decode, uint8_t *nbits)
{
    bitstream_write_nbits(stream, symbole_decode, *nbits, 0);
}




//ecrit dans un fichier les codage AC d'une composante, après avoir codé les symboles avec huffman
void AC_composante_puis_huffman(struct bitstream *stream, int16_t *composante)
{
  // struct huff_table *mon_arbre = malloc(sizeof(struct huff_table));
  struct huff_table *mon_arbre = huffman_table_build(htables_nb_symb_per_lengths[1][0],
                      htables_symbols[1][0],
                      htables_nb_symbols[1][0]);
  int8_t compteur_zeros = 0;
  int8_t nb_zeros_finaux = 0;
  int8_t k = 63;

  while (composante[k] == 0 && k>=1)
  {
    printf("%d\n", composante[k]);
    nb_zeros_finaux++;
    k--;
  }
  for (int i = 1; i < 64 - nb_zeros_finaux; i++)
  {
    if (composante[i]==0)
    {
      compteur_zeros++;
      if (compteur_zeros == 16)
      {
        compteur_zeros = 0;
        uint8_t *nbits = malloc(sizeof(uint8_t));
        uint32_t symbole_decode = huffman_table_get_path(mon_arbre, 240, nbits);
        ecriture_symbole_AC(stream, symbole_decode, nbits);
      }
    }
    else
    {
      uint8_t magnitude = retourne_magnitude(composante[i]);
      int8_t bit = retourne_bits(composante[i], magnitude);
      //On veut concaténer
      uint8_t *nbits = malloc(sizeof(uint8_t));
      uint32_t symbole_decode = huffman_table_get_path(mon_arbre, compteur_zeros*16+magnitude, nbits);
      ecriture_symbole_AC(stream, symbole_decode, nbits);
      ecriture_symbole_AC(stream, bit, &magnitude);
      compteur_zeros = 0;
      free(nbits);
    }
  }
  if (nb_zeros_finaux != 0)
  {
    uint8_t *nbits = malloc(sizeof(uint8_t));
    uint32_t symbole_decode = huffman_table_get_path(mon_arbre, 0, nbits);
    ecriture_symbole_AC(stream, symbole_decode, nbits);
  }
}
