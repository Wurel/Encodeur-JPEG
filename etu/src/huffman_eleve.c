#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

struct huff_table_eleve{
  uint8_t profondeur;
  struct huff_table_eleve *pere;
  struct huff_table_eleve *fils[2];
  uint8_t bool_feuille;
  uint32_t valeur_feuille;

  uint8_t sature;
};
// void creation_et_ecrit(struct huff_table_eleve *arbre, uint32_t nombre_de_symbole_a_ecrire, uint8_t *symbols, uint8_t nb_symbols, uint8_t i, uint8_t *j);
// void creation_arbre(struct huff_table_eleve *arbre, uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols, uint8_t i, uint8_t *j);
struct huff_table_eleve *huffman_table_build_eleve(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols);
void creation_arbre(struct huff_table_eleve *arbre, uint8_t profondeur);
void sature_papa(struct huff_table_eleve *fils, struct huff_table_eleve *pere);
void ecriture(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols, struct huff_table_eleve  *racine);

uint32_t recherche_arbre(struct huff_table_eleve *ht, uint8_t value, uint8_t *nbits, uint32_t* chemin, uint32_t* result_chemin);

static struct huff_table_eleve *racine= NULL;


struct huff_table_eleve *huffman_table_build_eleve(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols){
  racine = malloc(sizeof(struct huff_table_eleve*));
  racine -> pere = NULL;
  racine -> bool_feuille = 0;
  //rajout pour le patheleve
  racine -> profondeur = 0;
  // racine -> fils = malloc(2*sizeof(struct huff_table_eleve*));
  // struct huff_table_eleve *fils_0 = malloc(sizeof(struct huff_table_eleve));
  // // fils_0 -> profondeur = (arbre -> profondeur) + 1;
  // // printf("prof %d\n", fils_0 -> profondeur);
  // fils_0 -> feuille = 0;
  // fils_0 -> pere = racine;
  // racine->fils[0] = fils_0;
  //
  // struct huff_table_eleve *fils_1 = malloc(sizeof(struct huff_table_eleve));
  // fils_1 -> feuille = 0;
  // fils_1 -> pere = racine;
  // racine->fils[1] = fils_1;

  // racine -> profondeur = 0;
  uint8_t *j = malloc(sizeof(uint8_t));
  *j = 0;
  creation_arbre(racine, 0);
  ecriture(nb_symb_per_lengths, symbols, nb_symbols, racine);
  return racine;
}

void creation_arbre(struct huff_table_eleve *arbre, uint8_t profondeur){
  if (profondeur == 16) {
    return;
  }
  else{
    struct huff_table_eleve *fils_0 = malloc(sizeof(struct huff_table_eleve));
    fils_0 -> bool_feuille = 0;
    fils_0 -> pere = arbre;
    fils_0 -> profondeur = profondeur+1;
    fils_0 -> sature = 0;
    arbre -> fils[0] = fils_0;


    struct huff_table_eleve *fils_1 = malloc(sizeof(struct huff_table_eleve));
    fils_1 -> bool_feuille = 0;
    fils_1 -> pere = arbre;
    fils_1 -> profondeur = profondeur+1;
    fils_1 -> sature = 0;
    arbre -> fils[1] = fils_1;

    creation_arbre(fils_0, profondeur +1);
    creation_arbre(fils_1, profondeur +1);
  }
}

void ecriture(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols, struct huff_table_eleve  *racine) {
  struct huff_table_eleve *courant = malloc(sizeof(struct huff_table_eleve));
  uint32_t elements_ecrits = 0;
  printf("coucou\n");
  for (uint8_t i = 0; i < 16; i++) {
    for (size_t j = 0; j < nb_symb_per_lengths[i]; j++) {
      if (elements_ecrits == nb_symbols) {
        return;
      }
      courant = racine;
      while (courant -> profondeur != i + 1 && courant -> fils[0] != NULL) {
        if (courant ->fils[0] -> sature == 0) {
          courant = courant -> fils[0];
        }
        else{
          courant = courant -> fils[1];
        }
      }
      // printf("j'ecris %d\n", symbols[elements_ecrits]);
      courant -> valeur_feuille = symbols[elements_ecrits];
      courant -> bool_feuille = 1;
      courant -> sature = 1;
      elements_ecrits ++;
      sature_papa(courant, courant -> pere);
    }
  }
}

void sature_papa(struct huff_table_eleve *fils, struct huff_table_eleve *pere) {
  if(fils == (pere->fils[1])){
    pere -> sature = 1;
    sature_papa(pere, pere -> pere);
  }
}

/*
    Retourne le chemin dans l'arbre ht permettant d'atteindre la feuille de
    valeur value. nbits est un paramètre de sortie permettant de stocker la
    longueur du chemin.
*/

uint32_t huffman_table_get_path_eleve(struct huff_table_eleve *ht, uint8_t value, uint8_t *nbits){
  uint32_t *chemin = malloc(sizeof(uint32_t));
  uint32_t *result_chemin = malloc(sizeof(uint32_t));

  //printf("le chemin est %d\n", *chemin);

  struct huff_table_eleve *courant = malloc(sizeof(struct huff_table_eleve *));
  courant = ht;
  uint32_t valeur_codee = recherche_arbre(courant, value, nbits, chemin, result_chemin);
  printf("dans le get_path:\n");
  printf("valeur codee est de %d sur %d\n", *result_chemin, *nbits );
  //printf("le chemin est %d\n", valeur_codee);
  printf("\n");
  return *result_chemin;
}

uint32_t recherche_arbre(struct huff_table_eleve *courant, uint8_t value, uint8_t *nbits, uint32_t *chemin,
                                 uint32_t *result_chemin){
  //condition d'arret:
  if (courant -> bool_feuille == 1 && courant -> valeur_feuille == value)
  {
    *nbits = courant -> profondeur;
    printf("trouve\n");
    printf("dans le recherche_arbre:\n" );
    printf("n bit est de %d\n",*nbits );
    printf("le chemin est %d\n", *chemin);
    printf("\n");
    *result_chemin = *chemin;
    printf("\n");
    return *chemin;
  }
  if (courant ->bool_feuille == 1)
  {
    //resolution du probleme de recurssion
    *chemin = *chemin /2;
  }

  if(courant -> bool_feuille != 1 && courant -> profondeur != 16)
  {
    //petite verif (potentielle erreur à cause de la recursion pour l'initialisation de chemin)
    if (courant->profondeur == 0)
    {
      *chemin = 0;
    }
    //recherche a gauche:
    uint32_t chemin_gauche = (*chemin * 2) +0;
    *chemin = chemin_gauche;
    recherche_arbre(courant->fils[0], value, nbits, chemin, result_chemin);
    //recherche a droite:
    uint32_t chemin_droit = (*chemin * 2) +1;
    *chemin = chemin_droit;
    recherche_arbre(courant->fils[1], value, nbits, chemin, result_chemin);
    *chemin = *chemin / 2; //resolution probleme de recurssion
  }
}

// 
//
// int main() {
//   uint8_t nb_symb_per_lengths[5] = {0,2,1,5};
//   uint8_t symbols[8] = {5,4,8,16,32,54,11,12};
//   racine = huffman_table_build_eleve(nb_symb_per_lengths, symbols, 8); //le 8 est nb_symbols
//   uint8_t *nbits = malloc(sizeof(uint8_t));
//   printf("dans le main: \n" );
//   printf("on obtient %d\n", huffman_table_get_path_eleve(racine, 12, nbits));
//   printf("codé sur %d bits \n", *nbits);
//   if (racine -> pere == NULL) {
//     printf("c'est nul\n");
//   }
//   printf("fin\n");
//   // printf("valeur_feuille %d\n", racine->fils[0]->fils[0]->valeur_feuille);
//   return 0;
// }
