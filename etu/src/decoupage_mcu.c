#include "decoupage_mcu.h"
#include "structures.h"

uint8_t type(const char *ppm_filename){
  char *type;
  type = malloc(100*sizeof(char));
  FILE *ppm;
  ppm = fopen(ppm_filename, "r");
  if (ppm==NULL)
  {
    printf("Ouverture du fichier impossible\n");
    exit(0);
  }
  fscanf(ppm, "%s", type);
  printf("%s\n", type);
  fclose(ppm);
  if (!strcmp(type, "P5"))
  {
    free(type);
    return 1;
  }
  else{
    free(type);
    return 3;
  }



}


uint8_t *recuperation_rgb(const char *ppm_filename)
{
  char *type;
  type = malloc(100*sizeof(char));
  // char *largeur;
  // largeur = malloc(sizeof(char));
  // char *hauteur;
  // hauteur = malloc(sizeof(char));
  char *poubelle;
  poubelle = malloc(50*sizeof(char));
  uint32_t largeur;
  uint32_t hauteur;
  FILE *ppm;
  ppm = fopen(ppm_filename, "r");
  if (ppm==NULL)
  {
    printf("Ouverture du fichier impossible\n");
    exit(0);
  }
  /* recupere les parametres pour creer un tableau adapte */
  fscanf(ppm, "%s", type);
  fscanf(ppm, "%d", &largeur);
  fscanf(ppm, "%d", &hauteur);
  fread(poubelle, 1, 5, ppm);
  // fprintf(stderr, "erreur\n");
  if (!strcmp(type, "P5"))
  {
    uint8_t *tableau = malloc(sizeof(uint8_t)*(hauteur*largeur + 3));
    printf("negro\n");
    // 1 car 1 pixel = 1 byte
    tableau[0] = 1;


    //ATTENTION BIDOUILLE


    tableau[1] = 0;
    tableau[2] = 0;
    fread(tableau + 3, 1, largeur*hauteur, ppm);
    fclose(ppm);
    free(type);
    // free(largeur);
    // free(hauteur);
    free(poubelle);
    return tableau;
  }
  if (!strcmp(type, "P6"))
  {
    printf("image en couleur\n");
    uint8_t *tableau = malloc(sizeof(uint8_t)*(3*hauteur*largeur + 3));
    // 1 car 1 pixel = 1 byte
    tableau[0] = 0;


    //ATTENTION BIDOUILLE


    tableau[1] = 0;
    tableau[2] = 0;
    fread(tableau + 3, 1, 3*largeur*hauteur, ppm);
    fclose(ppm);
    free(type);
    // free(largeur);
    // free(hauteur);
    free(poubelle);
    return tableau;
  }
  printf("mauvais type d'image\n");
  exit(1);
}

uint32_t ajustement_taille(int32_t taille)
{
  uint32_t nouvelle_taille = taille;
  while (nouvelle_taille % 8 != 0) {
    nouvelle_taille ++;
  }
  return nouvelle_taille;
}


uint8_t *rgb_rembourre(const char *ppm_filename, int8_t h1, int8_t v1)
{
  uint32_t hauteur = taille_tableau_x8(ppm_filename, h1, v1)[1];
  uint32_t largeur = taille_tableau_x8(ppm_filename, h1, v1)[0];
  uint32_t hauteur_objectif = ajustement_taille(hauteur);
  uint32_t largeur_objectif = ajustement_taille(largeur);
  uint8_t * rgb_bonne_taille = malloc((hauteur_objectif*largeur_objectif+3)*sizeof(uint8_t));
  uint8_t *tableau_rgb = malloc(sizeof(uint8_t)*(hauteur * largeur + 3));
  tableau_rgb = recuperation_rgb(ppm_filename);
  rgb_bonne_taille[0] = tableau_rgb[0];
  for (uint32_t i = 0; i < hauteur_objectif; i++) {
    for (uint32_t j = 0; j < largeur_objectif; j++) {
      if (j > largeur ) {
        rgb_bonne_taille[i*largeur_objectif + j+3] = tableau_rgb[i*largeur + largeur + 3];
      }
      else if (i > hauteur){
        rgb_bonne_taille[i*largeur_objectif + j+3] = tableau_rgb[i*largeur + hauteur + 3];
      }
      else{
        rgb_bonne_taille[i*largeur_objectif + j+3] = tableau_rgb[i * largeur + j + 3];
      }
    }
  }
  free(tableau_rgb);
  return rgb_bonne_taille;
}

uint32_t *taille_tableau_x8(const char * ppm_filename, int8_t h1, int8_t v1)
{
  if (h1 == 1 && v1 == 1)
  {
    char *type;
    type = malloc(100*sizeof(char));
    // char *largeur;
    // largeur = malloc(sizeof(char));
    // char *hauteur;
    // hauteur = malloc(sizeof(char));
    uint32_t largeur = 0;
    uint32_t hauteur = 0;
    FILE *ppm;
    ppm = fopen(ppm_filename, "r");
    if (ppm==NULL)
    {
      printf("Ouverture du fichier impossible\n");
      exit(0);
    }
    uint32_t *tab = malloc(2*sizeof(uint32_t));
    fscanf(ppm, "%s", type);
    fscanf(ppm, "%d", &largeur);
    fscanf(ppm, "%d", &hauteur);
    tab[0] = largeur;
    tab[1] = hauteur;
    fclose(ppm);
    free(type);
    // free(largeur);
    // free(hauteur);
    return tab;
  }
}




uint32_t *taille_tableau(const char * ppm_filename, int8_t h1, int8_t v1)
{
  if (h1 == 1 && v1 == 1)
  {
    char *type;
    type = malloc(100*sizeof(char));
    // char *largeur;
    // largeur = malloc(sizeof(char));
    // char *hauteur;
    // hauteur = malloc(sizeof(char));
    uint32_t largeur = 0;
    uint32_t hauteur = 0;
    FILE *ppm;
    ppm = fopen(ppm_filename, "r");
    if (ppm==NULL)
    {
      printf("Ouverture du fichier impossible\n");
      exit(0);
    }
    uint32_t *tab = malloc(2*sizeof(uint32_t));
    fscanf(ppm, "%s", type);
    fscanf(ppm, "%d", &largeur);
    fscanf(ppm, "%d", &hauteur);
    tab[0] = largeur/8;
    tab[1] = hauteur/8;
    fclose(ppm);
    free(type);
    return tab;
  }
}

//prend le fichier en entrée, le partitionne en une ou plusieurs MCU en RGB
struct mcu **decoupage_mc(const char *ppm_filename, int8_t h1, int8_t v1)
{
  if(h1 == 1 && v1 == 1)
  {
    uint32_t hauteur = taille_tableau_x8(ppm_filename, h1, v1)[1];
    uint32_t largeur = taille_tableau_x8(ppm_filename, h1, v1)[0];
    uint32_t hauteur_objectif = ajustement_taille(hauteur);
    uint32_t largeur_objectif = ajustement_taille(largeur);
    uint8_t * tab_rgb = malloc((hauteur_objectif*largeur_objectif+3)*sizeof(uint8_t));
    if (tab_rgb == NULL){
      printf("erreur d'allocation\n");
      exit(0);
    }
    tab_rgb = rgb_rembourre(ppm_filename, 1, 1);
    // tab_rgb = recuperation_rgb(ppm_filename);
      if (largeur_objectif % 8 == 0 && hauteur_objectif % 8 ==0) // on fait juste le cas multiple de 8
      {
        struct mcu **tableau_de_mcu;
        tableau_de_mcu = malloc(hauteur_objectif/8*sizeof(*tableau_de_mcu));
        if (tableau_de_mcu == NULL){
          printf("erreur d'allocation\n");
          exit(0);
        }
        for (uint32_t i = 0; i < hauteur_objectif/8; i++) {
          tableau_de_mcu[i] = malloc(largeur_objectif/8 * sizeof(**tableau_de_mcu));
          if (tableau_de_mcu[i] == NULL){
            printf("erreur d'allocation\n");
            exit(0);
          }
        }
        for (uint32_t i = 0; i < hauteur_objectif/8; i++) {
          for (uint32_t j = 0; j < largeur_objectif/8; j++) {
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
              tableau_de_mcu[i][j].tableau_de_bloc[k].rgb = malloc((hauteur_objectif*largeur_objectif)*sizeof(uint32_t));
              tableau_de_mcu[i][j].tableau_de_bloc[k].y = malloc(64*sizeof(uint8_t));
              tableau_de_mcu[i][j].tableau_de_bloc[k].cb = malloc(64*sizeof(uint8_t));
              tableau_de_mcu[i][j].tableau_de_bloc[k].cr = malloc(64*sizeof(uint8_t));
              tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].y = malloc(64*sizeof(uint16_t));
              tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cr = malloc(64*sizeof(uint16_t));
              tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cb = malloc(64*sizeof(uint16_t));
            }
          }
        }
        if (tab_rgb[0] == 1)
        //cas négro
        {
        int32_t indice_ligne = 0;
        for (uint32_t k = 0; k < hauteur_objectif*largeur_objectif; k++)
        {
          if (k % largeur_objectif == 0 && (k != 0)) {
            indice_ligne ++;
          }
          if (indice_ligne == 8) {
            indice_ligne = 0;
          }
          uint32_t i = k/largeur_objectif;
          uint32_t j = k%largeur_objectif;
          // uint32_t j = k%hauteur_objectif;
          // printf("%d\n", k);
          // int32_t i_prime = k/hauteur - 8*i;
          // int32_t j_prime = k%hauteur -j*8;
          int32_t i_prime = i-8*(k/largeur_objectif/8);
          // int32_t j_prime = j-8*(k%largeur_objectif/8);
          int32_t j_prime = j*8 + k%8;
          int32_t nombre;
          nombre = tab_rgb[k + 3] + tab_rgb[k + 3]*16*16 + tab_rgb[k + 3]*16*16*16*16;
          tableau_de_mcu[i/8][j/8].tableau_de_bloc[0].rgb[k%8+indice_ligne*8] = nombre;
          // tableau_de_mcu[k/largeur_objectif/8][k%largeur_objectif/8].tableau_de_bloc[0].rgb[i_prime*8+j_prime] = nombre;
        }
      }
        if (tab_rgb[0] == 0)
        {
          printf("coul\n");
        int32_t indice_ligne = 0;
        for (uint32_t k = 0; k < hauteur_objectif*largeur_objectif; k++)
        {
          if (k % largeur_objectif == 0 && (k != 0)) {
            indice_ligne ++;
          }
          if (indice_ligne == 8) {
            indice_ligne = 0;
          }
          uint32_t i = k/largeur_objectif;
          uint32_t j = k%largeur_objectif;
          // uint32_t j = k%hauteur_objectif;
          // printf("%d\n", k);
          // int32_t i_prime = k/hauteur - 8*i;
          // int32_t j_prime = k%hauteur -j*8;
          int32_t i_prime = i-8*(k/largeur_objectif/8);
          // int32_t j_prime = j-8*(k%largeur_objectif/8);
          int32_t j_prime = j*8 + k%8;
          int32_t nombre;
          nombre = tab_rgb[3*k + 3] + tab_rgb[3*k + 1 + 3]*16*16 + tab_rgb[3*k + 2 + 3]*16*16*16*16;
          tableau_de_mcu[i/8][j/8].tableau_de_bloc[0].rgb[k%8+indice_ligne*8] = nombre;
          // tableau_de_mcu[k/largeur_objectif/8][k%largeur_objectif/8].tableau_de_bloc[0].rgb[i_prime*8+j_prime] = nombre;
        }
      }
      free(tab_rgb);
      return tableau_de_mcu;
      }
    }
  }
