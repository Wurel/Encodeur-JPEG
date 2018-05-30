#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "huffman_eleve.h"

/*
    Construit un arbre de Huffman à partir d'une table de symboles comme
    présenté en section 2.10.1 du sujet. nb_symb_per_lengths est un tableau
    contenant le nombre de symboles pour chaque longueur, symbols est le
    tableau des symboles ordonnés, et nb_symbols représente la taille du
    tableau symbols.
*/
struct huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths, uint8_t *symbols)
{
  struct huff_table* racine = malloc(sizeof(struct huff_table));
  // racine -> pere = NULL;
  racine -> bool_feuille = 0;
  racine -> profondeur = 0;  //rajout pour le path
  racine->fils[0] = NULL;
  racine->fils[1] = NULL;

  uint32_t compteur = 0;
  for (uint32_t i = 0; i < 16; i++) {
    for (uint8_t k = 0; k < nb_symb_per_lengths[i]; k++) {
      place_element(racine, symbols[compteur], i+1);
      compteur +=1;
    }
  }
  return racine;
}

uint8_t place_element(struct huff_table *arbre, uint8_t symbole, uint8_t longueur_symbole)
{
  if (arbre->bool_feuille == 1)
  {
    return 0;
  }
  else
  {
    if (longueur_symbole == arbre->profondeur)
    {
      if (arbre->fils[0]!= NULL || arbre->fils[1] != NULL)
      {
        return 0;
      }
      else
      {
        arbre->bool_feuille = 1;
        arbre->valeur_feuille = symbole;
        return 1;
      }
    }
    else
    {
      if (arbre->fils[0] == NULL)
      {
        struct huff_table* fils_gauche = malloc(sizeof(struct huff_table));
        fils_gauche->fils[0] = NULL;
        fils_gauche->fils[1] = NULL;
        fils_gauche->bool_feuille = 0;
        fils_gauche->profondeur = arbre->profondeur + 1;
        arbre->fils[0] = fils_gauche;
      }
      uint8_t test = place_element(arbre->fils[0], symbole, longueur_symbole);
      if (test==1)
      {
        return 1;
      }
      else
      {
        if (arbre->fils[1] == NULL)
        {
          struct huff_table* fils_droit = malloc(sizeof(struct huff_table));
          fils_droit->fils[0] = NULL;
          fils_droit->fils[1] = NULL;
          fils_droit->bool_feuille = 0;
          fils_droit->profondeur = arbre->profondeur + 1;
          arbre->fils[1] = fils_droit;
        }
        uint8_t test2 = place_element(arbre->fils[1], symbole, longueur_symbole);
        if (test2==1)
        {
          return 1;
        }
        else
        {
          return 0;
        }
      }
    }
  }
}

//
//     Construit un arbre de Huffman à partir d'une table de symboles comme
//     présenté en section 2.10.1 du sujet. nb_symb_per_lengths est un tableau
//     contenant le nombre de symboles pour chaque longueur, symbols est le
//     tableau des symboles ordonnés, et nb_symbols représente la taille du
//     tableau symbols.
// */
uint32_t huffman_table_get_path(struct huff_table *ht, uint8_t value, uint8_t *nbits){
  uint32_t *chemin = NULL;
  chemin = malloc(sizeof(uint32_t));
  *chemin = 0;
  recherche_chemin(ht, value, nbits, chemin,0);
  uint32_t reponse = *chemin;
  free(chemin);
  return reponse;
}

uint8_t recherche_chemin(struct huff_table *ht, uint8_t value, uint8_t *nbits, uint32_t* chemin, uint8_t gauche_ou_droite)
{
  if (ht->bool_feuille == 1)
  {
    if (ht->valeur_feuille == value)
    {
      *nbits = ht->profondeur;
      return 1;
    }
    else
    {
      *chemin = (*chemin - gauche_ou_droite) / 2;
      return 0;
    }
  }
  else
  {
    uint8_t test;
    if (ht->fils[0] != NULL)
    {
      *chemin = *chemin * 2;
      test = recherche_chemin(ht->fils[0], value, nbits, chemin, 0);
    }
    else
    {
      test = 0;
    }
    if (test ==0)
    {
      uint8_t test2;
      if (ht->fils[1] != NULL)
      {
        *chemin = *chemin * 2 + 1;
        test2 = recherche_chemin(ht->fils[1], value, nbits, chemin, 1);
      }
      else
      {
        test2 = 0;
      }
      if (test2 ==0)
      {
        *chemin = (*chemin - gauche_ou_droite) / 2;
        return 0;
      }
      else
      {
        return 1;
      }
    }
    else
    {
      return 1;
    }
  }
}

void huffman_table_destroy(struct huff_table *ht)
{
  if (ht->fils[0] != NULL)
  {
    huffman_table_destroy(ht->fils[0]);
  }
  if (ht->fils[1] != NULL)
  {
    huffman_table_destroy(ht->fils[1]);
  }
  free(ht);
}



// main de test
// int main() {
//   printf("coucou c'est huffman\n");
//   uint8_t nb_symb_per_lengths[5] = {0,2,1,5};
//   uint8_t symbols[8] = {5,4,8,16,32,54,11,12};
//   struct huff_table *racine = NULL;
//   racine = huffman_table_build(nb_symb_per_lengths, symbols, 8); //le 8 est nb_symbols
//   uint8_t *nbits = malloc(sizeof(uint8_t));
//   printf("racine : %d\n", racine->fils[1]->fils[1]->fils[1]->fils[0]->valeur_feuille );
//   printf("on obtient %d\n", huffman_table_get_path(racine, 12, nbits));
//   printf("codé sur %d bits \n", *nbits);
//   huffman_table_destroy(racine);
// }
