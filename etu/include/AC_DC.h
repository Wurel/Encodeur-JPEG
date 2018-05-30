#ifndef AC_DC
#define AC_DC
#include "module_jpeg.h"
#include "structures.h"
#include "bitstream.h"
#include <stdint.h>


/*
  Retourne la magnitude d'un nombre entré en paramètre
*/
uint8_t retourne_magnitude(int16_t nombre);

/*
  Retourne la place d'un nombre entré en paramètre dans sa classe de magnitude
*/
int16_t retourne_bits(int16_t nombre, uint8_t magnitude);


/*
Pour les 3 fonctions suivantes:
Calcule le DC a l'aide du predicateur dans les cas Y, Cb ou Cr
et appele ecriture_symbole_DC pour le coder et l'ecrire dans le fichier
*/
void ecriture_DC_y(struct bitstream *stream, struct mcu **tab, uint32_t indice_i, uint32_t indice_j ,uint8_t indice_k, int16_t predicateur, struct jpeg_desc *jpeg);
void ecriture_DC_cb(struct bitstream *stream, struct mcu **tab, uint32_t indice_i, uint32_t indice_j ,uint8_t indice_k, int16_t predicateur, struct jpeg_desc *jpeg);
void ecriture_DC_cr(struct bitstream *stream, struct mcu **tab, uint32_t indice_i, uint32_t indice_j ,uint8_t indice_k, int16_t predicateur, struct jpeg_desc *jpeg);

/*
  Code et ecrit le nombre correspondant au DC mis en parametre
  dans le fichier lui aussi mis en parametre
  l'indice en parametre corespond a y cb cr  (0, 1, 2)
*/
void ecriture_symbole_DC(struct bitstream *stream, int16_t nombre, uint8_t indice, struct jpeg_desc *jpeg);

/*
  Encode les elements d'une composante avec huffman puis appele ecriture_symbole_AC
*/
void AC_composante_puis_huffman(struct bitstream *stream, int16_t *composante, uint8_t id_y_cb_cr, struct jpeg_desc *jpeg);


/*
  Ecrit le symbole AC (deja passé dans huffman) sur nbits dans le fichier en parametre
*/
void ecriture_symbole_AC(struct bitstream *stream, uint32_t symbole_decode, uint8_t *nbits);

/*
  Fonction complete qui ecrir l'AC/DC qui prend en compte le daown sampling
  et le type (N&b ou couleur) de l'image
*/
void ecriture_AC_DC_complete(struct bitstream *stream, struct mcu **tab, uint32_t h, uint32_t v, uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3,  uint8_t type, struct jpeg_desc *jpeg);


#endif /* AC_DC */
