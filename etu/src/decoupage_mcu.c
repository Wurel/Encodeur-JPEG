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

//Taille en pixels avec rembourrage
uint32_t ajustement_taille(int32_t taille, uint8_t h1ouv1)
{
  uint32_t nouvelle_taille = taille;
  while (nouvelle_taille % (8 * h1ouv1) != 0) {
    nouvelle_taille ++;
  }
  return nouvelle_taille;
}


uint8_t *rgb_rembourre(const char *ppm_filename, uint8_t h1, uint8_t v1)
{
  uint32_t * tab_taille_x8 = malloc(2*sizeof(uint32_t));
  tab_taille_x8 = taille_tableau_x8(ppm_filename);
  uint32_t hauteur = tab_taille_x8[1];
  uint32_t largeur = tab_taille_x8[0];
  free(tab_taille_x8);
  uint32_t hauteur_objectif = ajustement_taille(hauteur, v1);
  uint32_t largeur_objectif = ajustement_taille(largeur, h1);
  uint8_t * rgb_bonne_taille = malloc((3*hauteur_objectif*largeur_objectif+3)*sizeof(uint8_t));
  uint8_t *tableau_rgb = NULL;
  tableau_rgb = recuperation_rgb(ppm_filename);
  rgb_bonne_taille[0] = tableau_rgb[0];
  if (tableau_rgb[0] == 1) {
    for (uint32_t i = 0; i < hauteur_objectif; i++) {
      for (uint32_t j = 0; j < largeur_objectif; j++) {
        // if (j >= largeur) {
        //   rgb_bonne_taille[i*largeur_objectif + j+3] = 0;
        // }
        // else if (i >= hauteur){
        //   rgb_bonne_taille[i*largeur_objectif + j+3] = 0;
        // }
        if(j < largeur && i < hauteur){
          rgb_bonne_taille[i*largeur_objectif + j+3] = tableau_rgb[i * largeur + j + 3];
        }
        else{
          rgb_bonne_taille[i*largeur_objectif + j+3] = 0;
        }
      }
    }
  }
  else{
    for (uint32_t i = 0; i < hauteur_objectif; i++) {
      for (uint32_t j = 0; j < largeur_objectif; j++) {
        // if (j >= largeur && i < hauteur) {
        //   rgb_bonne_taille[i*largeur_objectif + 3*j+3] = 0;
        //   rgb_bonne_taille[i*largeur_objectif + 3*j+3+1] = 0;
        //   rgb_bonne_taille[i*largeur_objectif + 3*j+3+2] = 0;
        // }
        // if (i >= hauteur && j < largeur){
        //   // ATTENTION potentiellement probleme
        //
        //
        //   //ATTENTION
        //   rgb_bonne_taille[i*largeur_objectif + 3*j+3] = 0;
        //   rgb_bonne_taille[i*largeur_objectif + 3*j+3+1] = 0;
        //   rgb_bonne_taille[i*largeur_objectif + 3*j+3+2] = 0;
        // }
        // if (i > hauteur && j > largeur) {
        //   rgb_bonne_taille[i*largeur_objectif + 3*j+3] = 0;
        //   rgb_bonne_taille[i*largeur_objectif + 3*j+3+1] = 0;
        //   rgb_bonne_taille[i*largeur_objectif + 3*j+3+2] = 0;
        // }
        if (j< largeur && i < hauteur){
          rgb_bonne_taille[i*largeur_objectif*3 + 3*j+3] = tableau_rgb[i * largeur*3 + 3*j + 3];
          rgb_bonne_taille[i*largeur_objectif*3 + 3*j+3+1] = tableau_rgb[i * largeur*3 + 3*j + 3+1];
          rgb_bonne_taille[i*largeur_objectif*3 + 3*j+3+2] = tableau_rgb[i * largeur*3 + 3*j + 3+2];
        }
        else{
          rgb_bonne_taille[i*largeur_objectif*3 + 3*j+3] = 0;
          rgb_bonne_taille[i*largeur_objectif*3 + 3*j+3+1] = 0;
          rgb_bonne_taille[i*largeur_objectif*3 + 3*j+3+2] = 0;
        }
      }
    }
  }
  // for (uint32_t i = 0; i < hauteur_objectif; i++) {
  //   for (uint32_t j = 0; j < largeur_objectif; j++) {
  //     printf("%x%x%x\n", rgb_bonne_taille[i*largeur_objectif*3 + 3*j+3], rgb_bonne_taille[i*largeur_objectif*3 + 3*j+3+1], rgb_bonne_taille[i*largeur_objectif*3 + 3*j+3+2]);
  //   }
  // }
  free(tableau_rgb);
  return rgb_bonne_taille;
}

//Retourne en pixels hauteur et largeur de l'image
uint32_t *taille_tableau_x8(const char * ppm_filename)
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
  exit(1);
}

//prend le fichier en entrée, le partitionne en une ou plusieurs MCU en RGB
struct mcu **decoupage_mc(const char *ppm_filename, uint8_t h1, uint8_t v1)
{
    uint32_t * tab_taille_x8 = malloc(2*sizeof(uint32_t));
    tab_taille_x8 = taille_tableau_x8(ppm_filename);
    uint32_t hauteur = tab_taille_x8[1];
    uint32_t largeur = tab_taille_x8[0];
    free(tab_taille_x8);
    uint32_t hauteur_objectif = ajustement_taille(hauteur, v1);
    uint32_t largeur_objectif = ajustement_taille(largeur, h1);
    // uint8_t * tab_rgb = malloc((3*hauteur_objectif*largeur_objectif+3)*sizeof(uint8_t));
    uint8_t *tab_rgb = NULL;
    // if (tab_rgb == NULL){
    //   printf("erreur d'allocation\n");
    //   exit(0);
    // }
    tab_rgb = rgb_rembourre(ppm_filename, h1, v1);
      //allocation memoire du tableau de mcu
      //tableau bidimensionnel (ligne colonne)
      if (largeur_objectif % 8*h1 == 0 && hauteur_objectif % 8*v1 ==0)//juste une verif
      {
        struct mcu **tableau_de_mcu;
        tableau_de_mcu = malloc(hauteur_objectif/8/v1*sizeof(*tableau_de_mcu));
        if (tableau_de_mcu == NULL){
          printf("erreur d'allocation\n");
          exit(0);
        }
        for (uint32_t i = 0; i < hauteur_objectif/(v1*8); i++) {
          tableau_de_mcu[i] = malloc(largeur_objectif/8/h1 * sizeof(**tableau_de_mcu));
          if (tableau_de_mcu[i] == NULL){
            printf("erreur d'allocation\n");
            exit(0);
          }
        }
        //allocation memoire des tableaux de blocs
        //tableau unidim (parcours de la lecture)
        for (uint32_t i = 0; i < hauteur_objectif/(8*v1); i++) {
          for (uint32_t j = 0; j < largeur_objectif/(8*h1); j++) {
            tableau_de_mcu[i][j].tableau_de_bloc = malloc(h1*v1*sizeof(struct bloc));
            if (tableau_de_mcu[i][j].tableau_de_bloc == NULL){
              printf("erreur d'allocation\n");
              exit(0);
            }
            //allocation des tableaux blocs apres dct
            //tableau unidim (parcours de la lecture)
            tableau_de_mcu[i][j].tableau_de_bloc_apres_dct = malloc(h1*v1*sizeof(struct bloc_apres_dct));
            if (tableau_de_mcu[i][j].tableau_de_bloc_apres_dct == NULL){
              printf("erreur d'allocation\n");
              exit(0);
            }
            tableau_de_mcu[i][j].h = h1;
            tableau_de_mcu[i][j].v = v1;
            //allocation des blocs

            for (uint8_t k = 0; k < h1*v1; k++) {
              struct bloc bloc0;
              tableau_de_mcu[i][j].tableau_de_bloc[k] = bloc0;
              struct bloc_apres_dct bloc1;
              tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k] = bloc1;
              //Allocation attributs des blocs
              tableau_de_mcu[i][j].tableau_de_bloc[k].rgb = malloc((hauteur_objectif*largeur_objectif)*sizeof(uint32_t)); //Allocation trop grande à 1ere vue
              tableau_de_mcu[i][j].tableau_de_bloc[k].y = malloc(64*sizeof(uint8_t));
              tableau_de_mcu[i][j].tableau_de_bloc[k].cb = malloc(64*sizeof(uint8_t));
              tableau_de_mcu[i][j].tableau_de_bloc[k].cr = malloc(64*sizeof(uint8_t));
              // tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].y = malloc(64*sizeof(uint16_t));
              // tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cr = malloc(64*sizeof(uint16_t));
              // tableau_de_mcu[i][j].tableau_de_bloc_apres_dct[k].cb = malloc(64*sizeof(uint16_t));
            }
          }
        }

        // int32_t indice_ligne = 0;
        for (uint32_t k = 0; k < hauteur_objectif*largeur_objectif; k++)
        {
            uint32_t i = k/largeur_objectif;
            uint32_t j = k%largeur_objectif;

            uint32_t i_mcu = i/8/v1;
            uint32_t j_mcu = j/8/h1;

            uint32_t i_bloc = (i - i_mcu * v1 * 8) / 8; //indice bloc dans un tableau de bloc comme si c'était bidimensionnel
            uint32_t j_bloc = (j - j_mcu * h1 * 8) / 8;
            uint32_t k_bloc = i_bloc * h1 + j_bloc; //On calcule à partir des "indices bidimensionnels" le vrai indice dans le tableau 1D

            uint8_t i_coefficient = i - i_mcu * v1 * 8 - i_bloc * 8;//indice coefficient dans un bloc comme si c'était bidimensionnel
            uint8_t j_coefficient = j - j_mcu * h1 * 8 - j_bloc * 8;
            uint32_t k_coefficient = 8 * i_coefficient + j_coefficient;//On calcule à partir des "indices bidimensionnels" le vrai indice dans le tableau 1D
            // int32_t i_prime = i-8*(k/largeur_objectif/8);
            // int32_t j_prime = j*8 + k%8;
            int32_t coefficient;
            if (tab_rgb[0] == 1)
            //cas négro
            {
              coefficient = tab_rgb[k + 3] + tab_rgb[k + 3]*16*16 + tab_rgb[k + 3]*16*16*16*16;
            }
            else
            {
              coefficient = tab_rgb[3*k + 3] + tab_rgb[3*k + 1 + 3]*16*16 + tab_rgb[3*k + 2 + 3]*16*16*16*16;
            }

            // printf("i : %d, j : %d \n\ni_mcu : %d, j_mcu : %d \nk_bloc : %d \nk_coefficient : %d\n\n\n ",i,j,i_mcu,j_mcu, k_bloc, k_coefficient );
            tableau_de_mcu[i_mcu][j_mcu].tableau_de_bloc[k_bloc].rgb[k_coefficient] = coefficient;
        }
      free(tab_rgb);
      return tableau_de_mcu;
      }
      exit(1);
  }
