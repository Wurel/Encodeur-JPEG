#ifndef AC_DC
#define AC_DC

#include "structures.h"
#include "bitstream.h"
#include <stdint.h>

int8_t retourne_magnitude(int16_t nombre);

int16_t retourne_bits(int16_t nombre, int8_t magnitude);

void ecriture(struct bitstream *stream, uint16_t nombre);

void AC_composante_puis_huffman(struct bitstream *stream, int16_t *composante);


#endif /* AC_DC */
