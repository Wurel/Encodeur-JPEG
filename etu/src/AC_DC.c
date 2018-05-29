#include "AC_DC.h"
#include <math.h>
// #include "bitstream.h"
#include "module_bitstream.h"
#include <stdlib.h>
#include "huffman.h"
#include "htables.h"
#include <stdio.h>

uint8_t retourne_magnitude(int16_t nombre)
{
  for (uint8_t m = 0; m < 12; m++) {
    if ((-(pow(2,m)-1) <= nombre && nombre <= -pow(2,m-1)) || (pow(2,m-1) <= nombre && nombre <= pow(2,m)-1))
    {
      return m;
    }
  }
  return 0;
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



void ecriture_symbole_DC(struct bitstream *stream, int16_t nombre, uint8_t indice)
{
  uint32_t symbole_decode;
  struct huff_table *mon_arbre = huffman_table_build(htables_nb_symb_per_lengths[0][indice],
                      htables_symbols[0][indice],
                      htables_nb_symbols[0][indice]);
  uint8_t magnitude = retourne_magnitude(nombre);
  uint8_t *nbits = malloc(sizeof(uint8_t));
  symbole_decode = huffman_table_get_path(mon_arbre, magnitude, nbits);
  bitstream_write_nbits(stream, symbole_decode, *nbits, 0);
  free(nbits);
  int16_t bits = retourne_bits(nombre, magnitude);
  bitstream_write_nbits(stream, bits, magnitude, 0);
  huffman_table_destroy(mon_arbre);
}


//se fait appeler par la fonction AC DC et donne le symbole DC pour un indice de tableau de bloc donné
//Attention c'est dans le cas N&B
void ecriture_DC_y(struct bitstream *stream, struct mcu **tab, uint32_t indice_i, uint32_t indice_j ,uint8_t indice_k, int16_t predicateur)
{
  int16_t nombre_dc = tab[indice_i][indice_j].tableau_de_bloc_apres_dct[indice_k].y[0] - predicateur;
  ecriture_symbole_DC(stream, nombre_dc, 0);
}

void ecriture_DC_cb(struct bitstream *stream, struct mcu **tab, uint32_t indice_i, uint32_t indice_j , uint8_t indice_k, int16_t predicateur)
{
  int16_t nombre_dc = tab[indice_i][indice_j].tableau_de_bloc_apres_dct[indice_k].cb[0] - predicateur;
  ecriture_symbole_DC(stream, nombre_dc, 1);
}

void ecriture_DC_cr(struct bitstream *stream, struct mcu **tab, uint32_t indice_i, uint32_t indice_j ,uint8_t indice_k, int16_t predicateur)
{
  int16_t nombre_dc = tab[indice_i][indice_j].tableau_de_bloc_apres_dct[indice_k].cr[0] - predicateur;
  ecriture_symbole_DC(stream, nombre_dc, 2);
}




void ecriture_symbole_AC(struct bitstream *stream, uint32_t symbole_decode, uint8_t *nbits)
{
    bitstream_write_nbits(stream, symbole_decode, *nbits, 0);
}



//ecrit dans un fichier les codage AC d'une composante, après avoir codé les symboles avec huffman
void AC_composante_puis_huffman(struct bitstream *stream, int16_t *composante, uint8_t id_y_cb_cr)
{
  // struct huff_table *mon_arbre = malloc(sizeof(struct huff_table));
  struct huff_table *mon_arbre = huffman_table_build(htables_nb_symb_per_lengths[1][id_y_cb_cr],
                      htables_symbols[1][id_y_cb_cr],
                      htables_nb_symbols[1][id_y_cb_cr]);


  int8_t compteur_zeros = 0;
  int8_t nb_zeros_finaux = 0;
  int8_t k = 63;

  while (composante[k] == 0 && k>=1)
  {
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
        free(nbits);
      }
    }
    else
    {
      uint8_t magnitude = retourne_magnitude(composante[i]);
      int16_t bit = retourne_bits(composante[i], magnitude);
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
    free(nbits);
  }
  huffman_table_destroy(mon_arbre);
}




void ecriture_AC_DC_complete(struct bitstream *stream, struct mcu **tab, uint32_t h, uint32_t v, uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3, uint8_t type )
{
  //pour Y (cas de base)
  //(pour N&B il ne fait que cette première partie)
  if (type == 1)
  {
    int16_t predicateur = 0;
    for (uint32_t i = 0; i < v; i++)
    {
      for (uint32_t j = 0; j < h; j++)
      {
        //attention v1=h1=1
        // k = 0
        ecriture_DC_y(stream, tab, i, j, 0, predicateur);
        predicateur = tab[i][j].tableau_de_bloc_apres_dct[0].y[0];
        AC_composante_puis_huffman(stream, tab[i][j].tableau_de_bloc_apres_dct[0].y, 0);
      }
    }
  }

  if (type == 3)
  {
  //cas couleur sans down sampler
    int16_t predicateur_y = 0;
    int16_t predicateur_cb = 0;
    int16_t predicateur_cr = 0;
    for (uint32_t i = 0; i < v; i++)
    {
      for (uint32_t j = 0; j < h; j++)
      {
        for (uint8_t k = 0; k < h1*v1; k++) {
          ecriture_DC_y(stream, tab , i, j, k, predicateur_y);
          predicateur_y = tab[i][j].tableau_de_bloc_apres_dct[k].y[0];
          AC_composante_puis_huffman(stream, tab[i][j].tableau_de_bloc_apres_dct[k].y, 0);
        }

        for (uint8_t k = 0; k < h1*v1; k++) {
          ecriture_DC_cb(stream,tab ,i ,j ,k, predicateur_cb);
          predicateur_cb = tab[i][j].tableau_de_bloc_apres_dct[k].cb[0];
          AC_composante_puis_huffman(stream, tab[i][j].tableau_de_bloc_apres_dct[k].cb, 1);
          if(h2==h3 && h2<h1)
          {
              k+=(h1/h2)-1;
          }
          if(v2==v3 && v2<v1)
          {
              //On teste si on est en bout de ligne : si oui, on saute une ligne
              if ((k+1)%h1 == 0)
              {
                  k+=h1*v1/v2 -1;
              }
          }
        }

        for (uint8_t k = 0; k < h1*v1; k++) {
          ecriture_DC_cr(stream,tab ,i ,j ,k,  predicateur_cr);
          predicateur_cr = tab[i][j].tableau_de_bloc_apres_dct[k].cr[0];
          AC_composante_puis_huffman(stream, tab[i][j].tableau_de_bloc_apres_dct[k].cr, 2);
          if(h2==h3 && h2<h1)
          {
              k+=(h1/h2)-1;
          }
          if(v2==v3 && v2<v1)
          {
            //On teste si on est en bout de ligne : si oui, on saute une ligne
            if ((k+1)%h1 == 0)
            {
                k+=h1*v1/v2 -1;
            }
          }
        }
      }
    }
  }
}
