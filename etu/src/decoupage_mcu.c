#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
// #include "decoupage_mcu.h"



struct mcu{
  uint8_t h;
  uint8_t v;
  uint32_t rgb[64];
  int8_t y[64];
  int8_t cb[64];
  int8_t cr[64];
};

uint8_t *recuperation_rgb(const char *ppm_filename)
{
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
    uint8_t *tableau = malloc(atoi(hauteur)*atoi(largeur) + 3);
    // 1 car 1 pixel = 1 byte
    tableau[0] = 1;
    tableau[1] = atoi(largeur);
    tableau[2] = atoi(hauteur);
    fread(tableau + 3, 1, tableau[1]*tableau[2], ppm);
    fclose(ppm);
    return tableau;
  }
}


// int *tableau_decoupe(int*tab_rgb)
// {
//   if (tab_rgb[0]*tab_rgb[1]*tab_rgb[2] == 64)
//   {
//     return(tab_rgb);
//   }
// }


//prend le fichier en entrée, le partitionne en une ou plusieurs MCU en RGB
struct mcu **decoupage_mcu(const char *ppm_filename, int8_t h1, int8_t v1)
{
  if(h1 == 1 && v1 == 1)
  {
    uint8_t *tab_rgb = recuperation_rgb(ppm_filename);

    if (tab_rgb[0] == 1)
    //cas négro
    {
      if (tab_rgb[1] % 8 == 0 && tab_rgb[2] % 8 ==0) // on fait juste le cas multiple de 8
      {
        struct mcu **tableau_de_mcu;
        tableau_de_mcu = malloc(tab_rgb[2]/8*sizeof(*tableau_de_mcu));
        if (tableau_de_mcu == NULL){
          printf("erreur d'allocation\n");
          exit(0);
        }
        for (size_t i = 0; i < tab_rgb[2]/8; i++) {
          tableau_de_mcu[i] = malloc(tab_rgb[1]/8 * sizeof(**tableau_de_mcu));
          if (tableau_de_mcu[i] == NULL){
            printf("erreur d'allocation\n");
            exit(0);
          }
        }
        for (int32_t k = 0; k < tab_rgb[2]*tab_rgb[1]; k++)
        {
          int32_t i = (k/tab_rgb[2]-1)/8;
          int32_t j = (k/tab_rgb[2]-1)/8;
          int32_t i_prime = k/tab_rgb[2] - 8*i;
          int32_t j_prime = k%tab_rgb[2] -j*8;
          if (tableau_de_mcu == NULL){
            printf("erreur d'allocation\n");
            exit(0);
          }
          int32_t nombre;
          nombre = tab_rgb[k + 3] + tab_rgb[k + 3]*16*16 + tab_rgb[k + 3]*16*16*16*16;
          printf("salut\n");
          tableau_de_mcu[i][j].rgb[i_prime*8+j_prime] = nombre;
        }
        return tableau_de_mcu;
      }
    }
  }
}


// transforme UNE mcu en RGB en une MCU en YCbCr
// struct mcu transformation_rgb_ycbcr(struct mcu *mc)
// {
//
//   for (size_t i = 0; i < 64; i++) {
//     uint8_t rouge = mc.rgb[i_prime*8+j_prime][0];
//     uint8_t vert = mc.rgb[i_prime*8+j_prime][1];
//     uint8_t bleu = mc.rgb[i_prime*8+j_prime][2];
//     mc->y = 0.299*rouge + 0.587*vert + 0.114*bleu;
//     mc->cb = -0.1687*rouge + 0.3313*vert + 0.5*bleu +128;
//     mc->cr = 0.299*rouge + 0.587*vert - 0.0813*bleu +128;
//
//   }
//
// }



// struct mcu *create_mcu(const char *ppm_filename)
// {
//
// }



// struct mcu *renvoie_mcu_8_8(*tableau_decoupe)
// {
//   if(tableau_decoupe[0] == 1)
//   {
//     struct mcu *
//   }
// }

int main(int argc, char const *argv[]) {
  decoupage_mcu("/user/6/poraa/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/images/invader.pgm",1,1);
  return 0;
}
