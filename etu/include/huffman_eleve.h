#ifndef HUFFMAN_H_ELEVE
#define HUFFMAN_H_ELEVE


struct huff_table_eleve{
  uint8_t profondeur;
  struct huff_table_eleve *pere;
  struct huff_table_eleve *fils[2];
  uint8_t bool_feuille;
  uint32_t valeur_feuille;
  uint8_t sature;
};

struct huff_table_eleve *huffman_table_build_eleve(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols);

void creation_arbre(struct huff_table_eleve *arbre, uint8_t profondeur);

void sature_papa(struct huff_table_eleve *fils, struct huff_table_eleve *pere);

void ecriture(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols, struct huff_table_eleve  *racine);

#endif /* HUFFMAN_H_ELEVE */
