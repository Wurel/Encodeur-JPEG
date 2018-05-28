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

void recherche_arbre(struct huff_table_eleve *ht, uint8_t value, uint8_t *nbits, uint32_t* chemin);

static struct huff_table_eleve *racine= NULL;


struct huff_table_eleve *huffman_table_build_eleve(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols){
  racine = malloc(sizeof(struct huff_table_eleve*));
  racine -> pere = NULL;
  racine -> bool_feuille = 0;
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

uint32_t huffman_table_get_path_eleve(struct huff_table_eleve *ht, uint8_t value, uint8_t *nbits){
  uint32_t *chemin = malloc(sizeof(uint32_t));
  recherche_arbre(ht, value, nbits, chemin);
  return *chemin;
}

void recherche_arbre(struct huff_table_eleve *ht, uint8_t value, uint8_t *nbits, uint32_t *chemin){
  struct huff_table_eleve *courant = malloc(sizeof(struct huff_table_eleve *));
  courant = ht;
  if(courant -> bool_feuille != 1 && courant -> profondeur != 16){
    // printf("%d\n", chemin + (2<<courant->profondeur));
    recherche_arbre(courant->fils[0], value, nbits, chemin);
    chemin = chemin + (2<< courant -> profondeur);
    // printf("%d\n", *chemin);
    recherche_arbre(courant->fils[1], value, nbits, chemin );
  }
  if (courant -> bool_feuille == 1 && courant -> valeur_feuille == value) {
    nbits = &courant -> profondeur;
    printf("trouve\n");
    printf("%d\n", *chemin);
  }
}

// void creation_arbre(struct huff_table_eleve *arbre, uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols, uint8_t i, uint8_t *j){
//   if (i >= 16 || *j >= nb_symbols) {
//     return;
//   }
//   if (nb_symb_per_lengths[i] != 0){
//     int32_t nombre_remontee = log(nb_symb_per_lengths[i]+1)/log(2);
//     printf("%d\n", nombre_remontee);
//     while (nombre_remontee > 0 && arbre -> pere != NULL) {
//       arbre = arbre -> pere;
//       nombre_remontee --;
//       printf("ici\n");
//     }
//     creation_et_ecrit(arbre, nb_symb_per_lengths[i], symbols, nb_symbols, i, j);
//     creation_arbre(arbre, nb_symb_per_lengths, symbols, nb_symbols, i+1, j);
//   }
//   else{
//     // arbre -> fils = malloc(2*sizeof(struct huff_table_eleve*));
    // struct huff_table_eleve *fils_0 = malloc(sizeof(struct huff_table_eleve));
//     // fils_0 -> profondeur = (arbre -> profondeur) + 1;
//     // printf("prof %d\n", fils_0 -> profondeur);
//     fils_0 -> feuille = 0;
//     fils_0 -> pere = arbre;
//     arbre->fils[0] = fils_0;
//     printf("une creation\n");
//     // creation_arbre(fils_0, nb_symb_per_lengths, symbols, nb_symbols, i+1, j);
//
//     struct huff_table_eleve *fils_1 = malloc(sizeof(struct huff_table_eleve));
//     // fils_1 -> profondeur = (arbre -> profondeur) + 1;
//     fils_1 -> feuille = 0;
//     fils_1 -> pere = arbre;
//     arbre->fils[1] = fils_1;
//     printf("une autre creation\n");
//     creation_arbre(fils_0, nb_symb_per_lengths, symbols, nb_symbols, i+1, j);
//     creation_arbre(fils_1, nb_symb_per_lengths, symbols, nb_symbols, i+1, j);
//   }
// }




// void creation_et_ecrit(struct huff_table_eleve *arbre, uint32_t nombre_de_symbole_a_ecrire, uint8_t *symbols, uint8_t nb_symbols, uint8_t i, uint8_t *j){
//   // arbre -> fils = malloc(2*sizeof(struct huff_table_eleve*));
//   printf("creation_et_ecrit \n");
//   if (nombre_de_symbole_a_ecrire <= 2) {
//     struct huff_table_eleve *fils = malloc(sizeof(struct huff_table_eleve));
//     fils -> bool_feuille = 1;
//     printf("i = %d\n", i);
//     printf("j = %d\n", *j);
//     printf("symbols %d\n", symbols[*j]);
//     fils -> valeur_feuille = symbols[*j];
//     fils -> pere = arbre;
//     // fils -> profondeur = arbre -> profondeur + 1;
//     arbre -> fils[0] = fils;
//     printf("dans la fonction %d\n", arbre->fils[0]->valeur_feuille);
//     *j = *j + 1;
//
//     if (nombre_de_symbole_a_ecrire == 2) {
//       struct huff_table_eleve *fils_1 = malloc(sizeof(struct huff_table_eleve ));
//       fils_1 -> feuille = 1;
//       printf("i = %d\n", i);
//       printf("j = %d\n", *j);
//       printf("symbols %d\n", symbols[*j]);
//       fils_1 -> valeur_feuille = symbols[*j];
//       fils_1 -> pere = arbre;
//       // fils -> profondeur = arbre -> profondeur + 1;
//       arbre -> fils[1] = fils_1;
//       printf("dans la fonction %d\n", arbre->fils[1]->valeur_feuille);
//       *j = *j + 1;
//     }
//   }
//   else{
//     creation_et_ecrit(arbre -> fils[0], nombre_de_symbole_a_ecrire - nombre_de_symbole_a_ecrire/2, symbols, nb_symbols, i, j);
//     creation_et_ecrit(arbre -> fils[1], nombre_de_symbole_a_ecrire/2, symbols, nb_symbols, i, j);
//   }
// }

int main() {
  uint8_t nb_symb_per_lengths[5] = {0,2,1,3};
  uint8_t symbols[6] = {5,4,8,16,32,54};
  racine = huffman_table_build_eleve(nb_symb_per_lengths, symbols, 6);
  uint8_t *nbits = malloc(sizeof(uint8_t));
  printf("on obtient %d\n", huffman_table_get_path_eleve(racine, 16, nbits));
  printf("des bits %d\n", *nbits);
  if (racine -> pere == NULL) {
    printf("c'est nul\n");
  }
  printf("fin\n");
  // printf("valeur_feuille %d\n", racine->fils[0]->fils[0]->valeur_feuille);
  return 0;
}
