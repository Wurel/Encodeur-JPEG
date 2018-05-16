#include "decoupage_mcu.h"





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
struct mcu **decoupage_mc(const char *ppm_filename, int8_t h1, int8_t v1)
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
          tableau_de_mcu[i][j].rgb[i_prime*8+j_prime] = nombre;
          tableau_de_mcu[i][j].h = 1;
          tableau_de_mcu[i][j].v = 1;
        }
        return tableau_de_mcu;
      }
    }
  }
}

// transforme UNE mcu (son pointeur est mis en parametre) en RGB en une mcu en YCbCr
struct mcu transformation_rgb_ycbcr(struct mcu *mc)
{
  //parcours de chaque bloc dans la mcu
  for (size_t i = 0; i < mc->h; i++)
  {
    for (size_t j = 0; j < mc->v; j++)
    {
      //pour chaque tableau de composante de mcu ici juste un bloc pour la mcu
      for (size_t k = 0; k < 64; k++)
      {
        //mc est un triple bloc dans notre cas
        uint8_t rouge = mc->rgb[k]/(16*16*16*16);
        uint8_t vert = (mc->rgb[k]-rouge*(16*16*16*16))/(16*16);
        uint8_t bleu = mc->rgb[k]-rouge*(16*16*16*16)-vert*(16*16);
        printf("%x %x %x\n", rouge, vert, bleu);
        printf("%zu\n", k);
        mc->y[k] = 0.299*rouge + 0.587*vert + 0.114*bleu;
        mc->cb[k] = -0.1687*rouge - 0.3313*vert + 0.5*bleu +128;
        mc->cr[k] = 0.5*rouge + 0.4187*vert - 0.0813*bleu +128;
      }
    }
  }
}

//a modifier quand on aurra la s
void dct(uint8_t *composante)
{
  //changement d'intervalle: [0, 255] vers [-128, 127]
  for (size_t k = 0; k < 64; k++)
  {
    composante[k] = composante[k]-128.;
  }
  //transformee en cosinus discrete
  //copy de la composante
  uint8_t *copy_composante;
  copy_composante = malloc(sizeof(composante));
  memcpy(copy_composante, composante, 64);
  for (size_t k = 0; k < 64; k++)
  {
    //calcul de C(i) et C(j)
    uint8_t c_i = 1;
    uint8_t c_j =1;
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
      somme += copy_composante[p]*cos((2*x+1)*(k/8)*M_PI/16)*cos((2*y+1)*(k%8)*M_PI/16);
    }
    composante[k] = 4*c_i*c_j*somme;
  }
}




// }

//
// int main(int argc, char const *argv[]) {
  // transformation_rgb_ycbcr(&decoupage_mcu("/user/6/poraa/Downloads/Encodeur-JPEG-master/Encodeur-JPEG/etu/images/invader.pgm",1,1)[0][0]);
//   return 0;
// }
