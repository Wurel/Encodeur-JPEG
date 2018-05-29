#ifndef HUFFMAN_H
#define HUFFMAN_H



/* structure représentant un arbre de Huffman. */
struct huff_table{
  uint8_t profondeur;
  // struct huff_table *pere;
  struct huff_table *fils[2];
  uint8_t bool_feuille;
  uint32_t valeur_feuille;
  // uint8_t sature;
  // uint8_t nb_symbols;
};


/*
    Construit un arbre de Huffman à partir d'une table de symboles comme
    présenté en section 2.10.1 du sujet. nb_symb_per_lengths est un tableau
    contenant le nombre de symboles pour chaque longueur, symbols est le
    tableau des symboles ordonnés, et nb_symbols représente la taille du
    tableau symbols.
*/
struct huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols);


/*
    Retourne le chemin dans l'arbre ht permettant d'atteindre la feuille de
    valeur value. nbits est un paramètre de sortie permettant de stocker la
    longueur du chemin.
*/
uint32_t huffman_table_get_path(struct huff_table *ht, uint8_t value, uint8_t *nbits);

uint8_t recherche_chemin(struct huff_table *ht, uint8_t value, uint8_t *nbits, uint32_t* chemin);

uint8_t place_element(struct huff_table *arbre, uint8_t symbole, uint8_t longueur_symbole);

/*
    Détruit l'arbre de Huffman passé en paramètre et libère toute la mémoire
    qui lui est associée.
*/
void huffman_table_destroy(struct huff_table **ht, uint32_t* compteur);


#endif /* HUFFMAN_H */
