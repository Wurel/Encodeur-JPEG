#include "AC_DC.h"
#include <math.h>


int16_t retourne_magnitude_bits(int16_t nombre)
{
  for (int8_t m = 0; m < 12; m++) {
    if ((-(pow(2,m)-1) <= nombre && nombre <= -pow(2,m-1)) || (pow(2,m-1) <= nombre && nombre <= pow(2,m)-1))
    {
      printf("%d\n", m);
      if (nombre < 0)
      {
        int16_t retour = nombre + pow(2,m) - 1;
        printf("%d\n", retour);
      }
      else
      {
        int16_t retour = nombre;
        printf("%d\n", retour);
      }
    }
  }
}
