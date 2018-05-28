#ifndef HUFFMAN_H_ELEVE
#define HUFFMAN_H_ELEVE



/* structure représentant un arbre de Huffman. */
struct huff_table_eleve{
  uint8_t profondeur;
  struct huff_table_eleve *pere;
  struct huff_table_eleve *fils[2];
  uint8_t bool_feuille;
  uint32_t valeur_feuille;
  uint8_t sature;
};


/*
    Construit un arbre de Huffman à partir d'une table de symboles comme
    présenté en section 2.10.1 du sujet. nb_symb_per_lengths est un tableau
    contenant le nombre de symboles pour chaque longueur, symbols est le
    tableau des symboles ordonnés, et nb_symbols représente la taille du
    tableau symbols.
*/
struct huff_table_eleve *huffman_table_build_eleve(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols);


/* creer les noeuds de l'arbre de maniere recurssive */
void creation_arbre(struct huff_table_eleve *arbre, uint8_t profondeur);


/* ecrit les syboles aux emplacements voulus  */
void ecriture(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols, struct huff_table_eleve  *racine);

/*
    met le parametre sature du des noeud de l'arbre de huffman quand tout
    ses fils sont saturés (quand les feuille ont une valeur)
*/
void sature_papa(struct huff_table_eleve *fils, struct huff_table_eleve *pere);


/*
    Retourne le chemin dans l'arbre ht permettant d'atteindre la feuille de
    valeur value. nbits est un paramètre de sortie permettant de stocker la
    longueur du chemin.
*/
uint32_t huffman_table_get_path_eleve(struct huff_table_eleve *ht, uint8_t value, uint8_t *nbits);

/*
    parcours de l'arbre de maniere récursive pour trouver le symbole
    correspondant à la valeur donné
*/
uint32_t recherche_arbre(struct huff_table_eleve *courant, uint8_t value, uint8_t *nbits, uint32_t *chemin, uint32_t *result_chemin);

/*
    Retourne le tableau des symboles associé à l'arbre de Huffman passé en
    paramètre.
*/
uint8_t *huffman_table_get_symbols_eleve(struct huff_table_eleve *ht);

/*
    Retourne le tableau du nombre de symboles de chaque longueur associé à
    l'arbre de Huffman passé en paramètre.
*/
uint8_t *huffman_table_get_length_vector_eleve(struct huff_table_eleve *ht);

/*
    Détruit l'arbre de Huffman passé en paramètre et libère toute la mémoire
    qui lui est associée.
*/
void huffman_table_destroy_eleve(struct huff_table_eleve *ht);


#endif /* HUFFMAN_H_ELEVE */
