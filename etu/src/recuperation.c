#include "recuperation.h"



void recuperation_argument(int argc, char const *argv[], char sortie, uint8_t *sample)
{
  int choix = -1;
  for (int i = 0; i < argc; i++) {
    choix = strcmp("--help", argv[i]);
    if (choix == 0) {
      printf("--help pour afficher la liste des options acceptees\n" );
      printf("--outfile=sortie.jpg pour redefinir le nom du fichier de sortie\n");
      printf("--sample=h1xv1,h2xv2,h3xv3 pour definir les facteurs d'echantillonnage h x v des trois composantes de couleur\n");
      // break;
    }
    printf("%s\n", argv[i]);
  }
}
