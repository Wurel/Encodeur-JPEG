#ifndef decoupage_mcu
#define decoupage_mcu

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "structures.h"


/*
  renvoit 1 pour N&B et 3 pour couleur
*/
uint8_t type(const char *ppm_filename);

/*
   rembourre le tableau RGB pour qu'il est des tailles multiple de MCU
*/
uint8_t *rgb_rembourre(const char *ppm_filename, uint8_t h1, uint8_t v1);

/*
  donne la nouvelle taille de l'image avec les pixels remboures
  (en prenant en compte les possible mcu pas de taille 1x1)
*/
uint32_t ajustement_taille(int32_t taille, uint8_t h1ouv1);

/* Retourne en pixels la hauteur et la largeur de l'image */
uint32_t *taille_tableau_x8(const char * ppm_filename);

/*
  prend le fichier en entrée, le partitionne en une ou plusieurs MCU en RGB
*/
extern struct mcu **decoupage_mc(const char *ppm_filename, uint8_t h1, uint8_t v1);

/*
  renvoit le nombre de bloc en largeur et hauteur (nb de pixels /8)
*/
uint32_t *taille_tableau(const char * ppm_filename, int8_t h1, int8_t v1);

 /*
  recupere les composantes rgb dans le fichier mis en parametre
*/
extern uint8_t *recuperation_rgb(const char *ppm_filename);


#endif /* decoupage_mcu */
