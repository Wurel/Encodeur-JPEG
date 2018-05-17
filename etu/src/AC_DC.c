#include "AC_DC.h"
#include <math.h>
#include "bitstream.h"
#include <stdlib.h>


int8_t retourne_magnitude(int16_t nombre)
{
  for (int8_t m = 0; m < 12; m++) {
    if ((-(pow(2,m)-1) <= nombre && nombre <= -pow(2,m-1)) || (pow(2,m-1) <= nombre && nombre <= pow(2,m)-1))
    {
      return m;
    }
  }
  exit(0);
}

int16_t retourne_bits(int16_t nombre, int8_t magnitude)
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



void ecriture(struct bitstream *stream, uint16_t nombre)
{
  int8_t magnitude = retourne_magnitude(nombre);
  int16_t bits = retourne_bits(nombre, magnitude);
  bitstream_write_nbits(stream, magnitude, 4, 0);
  bitstream_write_nbits(stream, bits, magnitude + 1, 0);
}


uint16_t tempo_dc(uint16_t *composante)
{
  return composante[0];
}
