#ifndef q_zz
#define q_zz

#include <stdint.h>
#include "qtables.h"
#include "decoupage_mcu.h"

/*
  Renvoie un tableau passé en paramètre réordonné en zigzag dans un autre tableau
*/
extern int16_t* zigzag_composante(int16_t *composante);

/*
  Fait le calcul de quantification pour Y :
  Divise terme à terme le tableau de la composante, par la table de quantification
*/
extern void quantification_Y(int16_t *composante);

/*
  Fait le calcul de quantification pour Cb ou Cr :
  Divise terme à terme le tableau de la composante, par la table de quantification
*/
void quantification_Cb_Cr(int16_t *composante);


#endif /* q_zz */
