#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
// #include "decoupage_mcu.h"

struct MCU{
  uint8_t Y[64];
  uint8_t Cb[64];
  uint8_t Cr[64];
};


int *recuperation_rgb(const char *ppm_filename){
  // uint64_t type_noir;
  char *type;
  type = malloc(2*sizeof(char));
  char *largeur;
  largeur = malloc(sizeof(char));
  char *hauteur;
  hauteur = malloc(sizeof(char));
  FILE *ppm;
  ppm = fopen(ppm_filename, "r");
  uint8_t courant;
  if (ppm==NULL)
  {
    printf("Ouverture du fichier impossible\n");
    exit(0);
  }
  /* recupere les parametres pour creer un tableau adapte */
  fscanf(ppm, "%s", type);
  fscanf(ppm, "%s", largeur);
  fscanf(ppm, "%s", hauteur);
  fread(stderr, 1, 5, ppm);
  if (!strcmp(type, "P5"))
  {
    uint8_t tableau[atoi(hauteur)*atoi(largeur) + 3];
    tableau[0] = atoi(type);
    tableau[1] = atoi(largeur);
    tableau[2] = atoi(hauteur);
    fread(tableau+3, 1, tableau[1]*tableau[2], ppm);
    return tableau;
  }
  fclose(ppm);

}

int main(int argc, char const *argv[]) {
  recuperation_rgb("/user/6/poraa/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/images/invader.pgm");
  return 0;
}
