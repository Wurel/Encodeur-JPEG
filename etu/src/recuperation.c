#include "recuperation.h"



void recuperation_argument(int argc, char const *argv[], char *entree, char *sortie, uint8_t *sample)
{
  int choix = -1;
  for (int i = 0; i < argc; i++) {
    choix = strcmp("--help", argv[i]);
    if (choix == 0) {
      printf("--help pour afficher la liste des options acceptees\n" );
      printf("--outfile=sortie.jpg pour redefinir le nom du fichier de sortie\n");
      printf("--sample=h1xv1,h2xv2,h3xv3 pour definir les facteurs d'echantillonnage h x v des trois composantes de couleur\n");
    }
    else {
      if (strncmp("--outfile=", argv[i], 10)==0) {
        printf("--outfile\n");
        strcpy(sortie, &argv[i][10]);
        // printf("%s\n", sortie);
      }
      else if (strncmp("--sample=", argv[i], 9)==0){
        sample[0] = argv[i][9]-48;
        sample[1] = argv[i][11]-48;
        sample[2] = argv[i][13]-48;
        sample[3] = argv[i][15]-48;
        sample[4] = argv[i][17]-48;
        sample[5] = argv[i][19]-48;
      }
      else{
        strcpy(entree, argv[i]);
      }
    }
  }
  if (strcmp(sortie, "")==0){
    strncpy(sortie, entree, strlen(entree)-4);
    strcat(sortie, ".jpeg");
    // printf("%s\n", sortie);
  }
}
