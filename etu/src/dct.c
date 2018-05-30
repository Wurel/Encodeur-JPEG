#include "dct.h"


/*
  Transformation en cosinus discrete (DCT):
  prend en parametre la composante a transformer ainsi qu'une composante vide
  (qui deviendra la composante transformee)
*/
void dct_bloc(uint8_t *composante, int16_t *nouvelle_composante)
{
  //changement d'intervalle: [0, 255] vers [-128, 127]
  //transformee en cosinus discrete
  //copie de la composante
  for (int32_t k = 0; k < 64; k++)
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
    //calcul de la transformee avec changement d'intervalle
    float somme = 0;
    for (int32_t p = 0; p < 64; p++)
    {
      //i et j indices de l'ancienne composante, x et y de la nouvelle
      uint8_t x = p/8;
      uint8_t y = p%8;
      uint8_t i = k%8;
      uint8_t j = k/8;
      somme += ((float)composante[x+8*y]-128)*cos((2*x+1)*i*M_PI*0.0625)*cos((2*y+1)*j*M_PI*0.0625);
    }
    nouvelle_composante[k] = 0.25*c_i*c_j*somme;
  }
}
