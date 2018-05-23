#ifndef AC_DC
#define AC_DC

#include "structures.h"
#include "bitstream.h"
#include <stdint.h>


//Retourne la magnitude d'un nombre entré en paramètre
uint8_t retourne_magnitude(int16_t nombre);

//Retourne la place d'un nombre entré en paramètre dans sa classe de magnitude
int16_t retourne_bits(int16_t nombre, uint8_t magnitude);


void ecriture_symbole_DC(struct bitstream *stream, int16_t nombre, uint8_t indice);
void ecriture_DC_y(struct bitstream *stream, struct mcu **tab, uint8_t indice_i, uint8_t indice_j ,uint8_t indice_k, int16_t predicateur);
void ecriture_DC_cb(struct bitstream *stream, struct mcu **tab, uint8_t indice_i, uint8_t indice_j ,uint8_t indice_k, int16_t predicateur);
void ecriture_DC_cr(struct bitstream *stream, struct mcu **tab, uint8_t indice_i, uint8_t indice_j ,uint8_t indice_k, int16_t predicateur);

void AC_composante_puis_huffman(struct bitstream *stream, int16_t *composante, uint8_t id_y_cb_cr);

void ecriture_symbole_AC(struct bitstream *stream, uint32_t symbole_decode, uint8_t *nbits);

void ecriture_AC_DC_complete(struct bitstream *stream, struct mcu **tab, uint32_t h, uint32_t v, uint8_t h1, uint8_t v1, uint8_t type);

#endif /* AC_DC */
