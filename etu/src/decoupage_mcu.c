#include "decoupage_mcu.h"
#include "structures.h"




uint8_t *recuperation_rgb(const char *ppm_filename)
{
  uint32_t largeur1 = taille_tableau(ppm_filename, 1, 1)[0]*8;
  uint32_t hauteur1 = taille_tableau(ppm_filename, 1, 1)[1]*8;
  char *type;
  type = malloc(2*sizeof(char));
  char *largeur;
  largeur = malloc(sizeof(char));
  char *hauteur;
  hauteur = malloc(sizeof(char));
  char *poubelle;
  poubelle = malloc(50*sizeof(char));
  FILE *ppm;
  ppm = fopen(ppm_filename, "r");
  if (ppm==NULL)
  {
    printf("Ouverture du fichier impossible\n");
    exit(0);
  }
  /* recupere les parametres pour creer un tableau adapte */
  fscanf(ppm, "%s", type);
  fscanf(ppm, "%s", largeur);
  fscanf(ppm, "%s", hauteur);
  fread(poubelle, 1, 5, ppm);
  // fprintf(stderr, "erreur\n");
  if (!strcmp(type, "P5"))
  {
    uint8_t *tableau = malloc(sizeof(uint8_t)*(atoi(hauteur)*atoi(largeur) + 3));
    // 1 car 1 pixel = 1 byte
    tableau[0] = 1;


    //ATTENTION BIDOUILLE


    tableau[1] = 0;
    tableau[2] = 0;
    fread(tableau + 3, 1, largeur1*hauteur1, ppm);
    fclose(ppm);
    return tableau;
  }
}

uint32_t *taille_tableau(const char * ppm_filename, int8_t h1, int8_t v1)
{
  if (h1 == 1 && v1 == 1)
  {
    char *type;
    type = malloc(2*sizeof(char));
    char *largeur;
    largeur = malloc(sizeof(char));
    char *hauteur;
    hauteur = malloc(sizeof(char));
    FILE *ppm;
    ppm = fopen(ppm_filename, "r");
    if (ppm==NULL)
    {
      printf("Ouverture du fichier impossible\n");
      exit(0);
    }
    uint32_t *tab = malloc(2*sizeof(uint32_t));
    fscanf(ppm, "%s", type);
    fscanf(ppm, "%s", largeur);
    fscanf(ppm, "%s", hauteur);
    tab[0] = atoi(largeur)/8;
    tab[1] = atoi(hauteur)/8;
    fclose(ppm);
    return tab;
  }
}

//prend le fichier en entrée, le partitionne en une ou plusieurs MCU en RGB
struct mcu **decoupage_mc(const char *ppm_filename, int8_t h1, int8_t v1)
{
  uint32_t largeur = taille_tableau(ppm_filename, h1, v1)[0]*8;
  uint32_t hauteur = taille_tableau(ppm_filename, h1, v1)[1]*8;
  if(h1 == 1 && v1 == 1)
  {
    uint8_t *tab_rgb = malloc((largeur*hauteur + 3)*sizeof(uint8_t));
    if (tab_rgb == NULL){
      printf("erreur d'allocation\n");
      exit(0);
    }
    tab_rgb = recuperation_rgb(ppm_filename);
    if (tab_rgb[0] == 1)
    //cas négro
    {
      if (largeur % 8 == 0 && hauteur % 8 ==0) // on fait juste le cas multiple de 8
      {
        struct mcu **tableau_de_mcu;
        tableau_de_mcu = malloc(hauteur/8*sizeof(*tableau_de_mcu));
        if (tableau_de_mcu == NULL){
          printf("erreur d'allocation\n");
          exit(0);
        }
        for (size_t i = 0; i < hauteur/8; i++) {
          tableau_de_mcu[i] = malloc(largeur/8 * sizeof(**tableau_de_mcu));
          if (tableau_de_mcu[i] == NULL){
            printf("erreur d'allocation\n");
            exit(0);
          }
        }
        for (size_t i = 0; i < hauteur/8; i++) {
          for (size_t j = 0; j < largeur/8; j++) {
            tableau_de_mcu[i][j].tableau_de_bloc = malloc(h1*v1*sizeof(struct bloc));
            if (tableau_de_mcu[i][j].tableau_de_bloc == NULL){
              printf("erreur d'allocation\n");
              exit(0);
            }
            tableau_de_mcu[i][j].tableau_de_bloc_apres_dct = malloc(h1*v1*sizeof(struct bloc_apres_dct));
            if (tableau_de_mcu[i][j].tableau_de_bloc_apres_dct == NULL){
              printf("erreur d'allocation\n");
              exit(0);
            }
            struct bloc bloc0;
            tableau_de_mcu[i][j].tableau_de_bloc[0] = bloc0;
            struct bloc_apres_dct bloc1;
            tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[0] = bloc1;
            tableau_de_mcu[i][j].h = h1;
            tableau_de_mcu[i][j].v = v1;
            for (int k = 0; k < h1*v1; k++) {
              tableau_de_mcu[i][j].tableau_de_bloc[k].rgb = malloc(hauteur*largeur*sizeof(uint32_t));
              tableau_de_mcu[i][j].tableau_de_bloc[k].y = malloc(64*sizeof(uint8_t));
              tableau_de_mcu[i][j].tableau_de_bloc[k].cb = malloc(64*sizeof(uint8_t));
              tableau_de_mcu[i][j].tableau_de_bloc[k].cr = malloc(64*sizeof(uint8_t));
              tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].y = malloc(64*sizeof(uint16_t));
              tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cr = malloc(64*sizeof(uint16_t));
              tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cb = malloc(64*sizeof(uint16_t));
            }
          }
        }
        for (uint32_t k = 0; k < hauteur*largeur; k++)
        {
          uint32_t i = k/largeur;
          uint32_t j = k%hauteur;
          // int32_t i_prime = k/hauteur - 8*i;
          // int32_t j_prime = k%hauteur -j*8;
          int32_t i_prime = i-8*(k/largeur/8);
          int32_t j_prime = j-8*(k%largeur/8);
          int32_t nombre;
          nombre = tab_rgb[k + 3] + tab_rgb[k + 3]*16*16 + tab_rgb[k + 3]*16*16*16*16;
          tableau_de_mcu[k/largeur/8][k%largeur/8].tableau_de_bloc[0].rgb[i_prime*8+j_prime] = nombre;
        }
        return tableau_de_mcu;
      }
    }
  }
}
