// #include <stdlib.h>
// #include <stdint.h>
// #include <math.h>
// #include <stdio.h>
// #include "huffman_eleve.h"
//
// // /* structure représentant un arbre de Huffman. */
// // struct huff_table{
// //   uint8_t profondeur;
// //   // struct huff_table *pere;
// //   struct huff_table *fils[2];
// //   uint8_t bool_feuille;
// //   uint32_t valeur_feuille;
// //   // uint8_t sature;
// //   // uint8_t nb_symbols;
// // };
// //
// /*
//     Construit un arbre de Huffman à partir d'une table de symboles comme
//     présenté en section 2.10.1 du sujet. nb_symb_per_lengths est un tableau
//     contenant le nombre de symboles pour chaque longueur, symbols est le
//     tableau des symboles ordonnés, et nb_symbols représente la taille du
//     tableau symbols.
// */
// struct huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols)
// {
//   struct huff_table* racine = malloc(sizeof(struct huff_table*));
//   // racine -> pere = NULL;
//   racine -> bool_feuille = 0;
//   racine -> profondeur = 0;  //rajout pour le path
//   racine->fils[0] = NULL;
//   racine->fils[1] = NULL;
//
//   uint32_t compteur = 0;
//   for (uint32_t i = 0; i < 16; i++) {
//     for (uint8_t k = 0; k < nb_symb_per_lengths[i]; k++) {
//       uint8_t coucou = place_element(racine, symbols[compteur], i+1);
//       compteur +=1;
//     }
//   }
//   return racine;
// }
//
// uint8_t place_element(struct huff_table *arbre, uint8_t symbole, uint8_t longueur_symbole)
// {
//   if (arbre->bool_feuille == 1)
//   {
//     return 0;
//   }
//   else
//   {
//     if (longueur_symbole - 1 == arbre->profondeur)
//     {
//       if (arbre->fils[0]!= NULL || arbre->fils[1] != NULL)
//       {
//         return 0;
//       }
//       else
//       {
//         arbre->bool_feuille = 1;
//         arbre->valeur_feuille = symbole;
//         return 1;
//       }
//     }
//     else
//     {
//       if (arbre->fils[0] == NULL)
//       {
//         struct huff_table* fils_gauche = malloc(sizeof(struct huff_table*));
//         fils_gauche->fils[0] = NULL;
//         fils_gauche->fils[1] = NULL;
//         fils_gauche->bool_feuille = 0;
//         fils_gauche->profondeur = arbre->profondeur + 1;
//         arbre->fils[0] = fils_gauche;
//       }
//       uint8_t test = place_element(arbre->fils[0], symbole, longueur_symbole);
//       if (test==1)
//       {
//         return 1;
//       }
//       else
//       {
//         if (arbre->fils[1] == NULL)
//         {
//           struct huff_table* fils_droit = malloc(sizeof(struct huff_table*));
//           fils_droit->fils[0] = NULL;
//           fils_droit->fils[1] = NULL;
//           fils_droit->bool_feuille = 0;
//           fils_droit->profondeur = arbre->profondeur + 1;
//           arbre->fils[1] = fils_droit;
//         }
//         uint8_t test2 = place_element(arbre->fils[1], symbole, longueur_symbole);
//         if (test2==1)
//         {
//           return 1;
//         }
//         else
//         {
//           return 0;
//         }
//       }
//     }
//   }
// }
//
// //
// // /*
// //     Retourne le chemin dans l'arbre ht permettant d'atteindre la feuille de
// //     valeur value. nbits est un paramètre de sortie permettant de stocker la
// //     longueur du chemin.
// // */
// uint32_t huffman_table_get_path(struct huff_table *ht, uint8_t value, uint8_t *nbits){
//   uint32_t *chemin = malloc(sizeof(uint32_t));
//   *chemin = 0;
//   recherche_chemin(ht, value, nbits, chemin);
//   uint32_t reponse = *chemin;
//   free(chemin);
//   return reponse;
// }
//
// uint8_t recherche_chemin(struct huff_table *ht, uint8_t value, uint8_t *nbits, uint32_t* chemin)
// {
//   if (ht->bool_feuille == 1)
//   {
//     if (ht->valeur_feuille == value)
//     {
//       *nbits = ht->profondeur + 1;
//       return 1;
//     }
//     else
//     {
//       return 0;
//     }
//   }
//   else
//   {
//     *chemin = *chemin * 2;
//     uint8_t test;
//     if (ht->fils[0] != NULL)
//     {
//       test = recherche_chemin(ht->fils[0], value, nbits, chemin);
//     }
//     else
//     {
//       test = 0;
//     }
//     if (test ==0)
//     {
//       *chemin += 1;
//       uint8_t test2;
//       if (ht->fils[1] != NULL)
//       {
//         test2 = recherche_chemin(ht->fils[1], value, nbits, chemin);
//       }
//       else
//       {
//         test2 = 0;
//       }
//       if (test2 ==0)
//       {
//         return 0;
//         *chemin = (*chemin - 1) / 2;
//       }
//       else
//       {
//         return 1;
//       }
//     }
//     else
//     {
//       return 1;
//     }
//   }
// }
//
// void huffman_table_destroy(struct huff_table **ht, uint32_t* compteur)
// {
//   //descente
//   if (ht==NULL)
//   {
//     return;
//   }
//   else
//   {
//     if (*ht == NULL)
//     {
//       return;
//     }
//     printf("compteur : %d\n", *compteur);
//     *compteur = *compteur + 1;
//     if ((*ht)->fils[0]==NULL && (*ht)->fils[1]==NULL)
//     {
//       if (*ht!= NULL)
//       {
//         free(*ht);
//         *ht=NULL;
//       }
//     }
//     else
//     {
//       if ((*ht)->fils[0]!=NULL)
//       {
//         huffman_table_destroy(&((*ht)->fils[0]), compteur);
//       }
//       if ((*ht)->fils[1]!=NULL)
//       {
//         huffman_table_destroy(&((*ht)->fils[1]), compteur);
//       }
//       if (*ht!= NULL)
//       {
//         free(*ht);
//         *ht=NULL;
//       }
//
//     }
//   }
// }
//
//
// //main de test
// // int main() {
// //   uint8_t nb_symb_per_lengths[5] = {0,2,1,5};
// //   uint8_t symbols[8] = {5,4,8,16,32,54,11,12};
// //   racine = huffman_table_build(nb_symb_per_lengths, symbols, 8); //le 8 est nb_symbols
// //   uint8_t *nbits = malloc(sizeof(uint8_t));
// //   printf("dans le main: \n" );
// //   printf("on obtient %d\n", huffman_table_get_path(racine, 12, nbits));
// //   printf("codé sur %d bits \n", *nbits);
// //   if (racine -> pere == NULL) {
// //     printf("c'est nul\n");
// //   }
// // }
// //
// // /*
// // met le parametre sature du des huff_table de l'arbre de huffman quand tout
// // ses fils sont saturés (quand les feuille ont une valeur)
// // */
// // void sature_papa(struct huff_table *fils, struct huff_table *pere) {
// //   if(fils == (pere->fils[1])){
// //     pere -> sature = 1;
// //     sature_papa(pere, pere -> pere);
// //   }
// // }
// //
// // /*
// //     Retourne le chemin dans l'arbre ht permettant d'atteindre la feuille de
// //     valeur value. nbits est un paramètre de sortie permettant de stocker la
// //     longueur du chemin.
// // */
// // uint32_t huffman_table_get_path(struct huff_table *ht, uint8_t value, uint8_t *nbits){
// //   uint32_t *chemin = malloc(sizeof(uint32_t));
// //   uint32_t *result_chemin = malloc(sizeof(uint32_t));
// //
// //   //printf("le chemin est %d\n", *chemin);
// //
// //   struct huff_table *courant = malloc(sizeof(struct huff_table *));
// //   courant = ht;
// //   uint32_t valeur_codee = recherche_arbre(courant, value, nbits, chemin, result_chemin);
// //   printf("dans le get_path:\n");
// //   printf("valeur codee est de %d sur %d\n", *result_chemin, *nbits );
// //   //printf("le chemin est %d\n", valeur_codee);
// //   printf("\n");
// //   return *result_chemin;
// // }
// //
// // /* parcours de l'arbre de maniere récursive pour trouver le symbole correspondant à la valeur donné */
// // uint32_t recherche_arbre(struct huff_table *courant, uint8_t value, uint8_t *nbits, uint32_t *chemin,
// //                                  uint32_t *result_chemin){
// //   //condition d'arret:
// //   if (courant -> bool_feuille == 1 && courant -> valeur_feuille == value)
// //   {
// //     *nbits = courant -> profondeur;
// //     printf("trouve\n");
// //     printf("dans le recherche_arbre:\n" );
// //     printf("n bit est de %d\n",*nbits );
// //     printf("le chemin est %d\n", *chemin);
// //     printf("\n");
// //     *result_chemin = *chemin;
// //     printf("\n");
// //     return *chemin;
// //   }
// //   if (courant ->bool_feuille == 1)
// //   {
// //     //resolution du probleme de recurssion
// //     *chemin = *chemin /2;
// //   }
// //
// //   if(courant -> bool_feuille != 1 && courant -> profondeur != 16)
// //   {
// //     //petite verif (potentielle erreur à cause de la recursion pour l'initialisation de chemin)
// //     if (courant->profondeur == 0)
// //     {
// //       *chemin = 0;
// //     }
// //     //recherche a gauche:
// //     uint32_t chemin_gauche = (*chemin * 2) +0;
// //     *chemin = chemin_gauche;
// //     recherche_arbre(courant->fils[0], value, nbits, chemin, result_chemin);
// //     //recherche a droite:
// //     uint32_t chemin_droit = (*chemin * 2) +1;
// //     *chemin = chemin_droit;
// //     recherche_arbre(courant->fils[1], value, nbits, chemin, result_chemin);
// //     *chemin = *chemin / 2; //resolution probleme de recurssion
// //   }
// // }
// //
// // /*
// //     Retourne le tableau des symboles associé à l'arbre de Huffman passé en
// //     paramètre.
// // */
// // uint8_t *huffman_table_get_symbols(struct huff_table *ht)
// // {
// //   uint8_t *tab_symboles = malloc(sizeof(uint8_t)*(ht-> nb_symbols));
// //   return 0;
// // }
// //
// // /*
// // Détruit l'arbre de Huffman passé en paramètre et libère toute la mémoire
// // qui lui est associée.
// // */
// // void huffman_table_destroy(struct huff_table *ht)
// // {
// //   //descente
// //   if (ht->profondeur != 16)
// //   {
// //     huffman_table_destroy(ht->fils[0]);
// //     huffman_table_destroy(ht->fils[1]);
// //     free(ht);
// //   }
// //   else //remonte en faisant des frees
// //   {
// //     free(ht->fils[0]);
// //     free(ht->fils[1]);
// //   }
// // }
// //
// //
// // main de test
// // int main() {
// //   uint8_t nb_symb_per_lengths[3] = {0,2};
// //   uint8_t symbols[2] = {5,4};
// //   racine = huffman_table_build(nb_symb_per_lengths, symbols, 2); //le 8 est nb_symbols
// //   uint8_t *nbits = malloc(sizeof(uint8_t));
// //   printf("dans le main: \n" );
// //   printf("on obtient %d\n", huffman_table_get_path(racine, 5, nbits));
// //   printf("codé sur %d bits \n", *nbits);
// //   if (racine -> pere == NULL) {
// //     printf("c'est nul\n");
// //   }
// //   huffman_table_destroy(racine);
// //   printf("fin\n");
// //   // printf("valeur_feuille %d\n", racine->fils[0]->fils[0]->valeur_feuille);
// //   return 0;
// // }
