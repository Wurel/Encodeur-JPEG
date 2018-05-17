#include "dct.h"

void dct_bloc(uint8_t *composante, int16_t *nouvelle_composante)
{
  //changement d'intervalle: [0, 255] vers [-128, 127]
  //transformee en cosinus discrete
  //copy de la composante
  for (size_t k = 0; k < 64; k++)
  {
    //calcul de C(i) et C(j)
    float c_i = 1;
    float c_j =1;
    if (k/8 == 0)
    {
      c_i = 1/sqrt(2);
    }
    if (k%8 == 0)
    {
      c_j = 1/sqrt(2);
    }
    //calcul de la transformee
    float somme = 0;
    for (size_t p = 0; p < 64; p++)
    {
      uint8_t x = p/8;
      uint8_t y = p%8;
      uint8_t i = k%8;
      uint8_t j = k/8;
      // printf("%d, %d\n", y, j);
      // printf("%f\n", (2*y+1)*j*M_PI*0.0625);
      somme += ((float)composante[x+8*y]-128)*cos((2*x+1)*i*M_PI*0.0625)*cos((2*y+1)*j*M_PI*0.0625);
    }
    // printf("%f\n", 1/sqrt(2));
    // printf("%f\n", 0.25*c_i*c_j*somme);
    nouvelle_composante[k] = 0.25*c_i*c_j*somme;
    // printf("%hx, %zu\n", nouvelle_composante[k], k);
  }
}
