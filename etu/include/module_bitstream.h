#ifndef _bitstream_h_eleve_
#define _bitstream_h_eleve_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
    Type représentant le flux d'octets à écrire dans le fichier JPEG de
    sortie (appelé bitstream dans le sujet).
*/
struct bitstream_eleve;

/*
Retourne un nouveau bitstream prêt à écrire dans le fichier filename.
*/
extern struct bitstream_eleve *bitstream_create_eleve(const char *filename);

/*
Ecrit nbits bits dans le bitstream. La valeur portée par cet ensemble de
bits est value. Le paramètre is_marker permet d'indiquer qu'on est en train
d'écrire un marqueur de section dans l'entête JPEG ou non (voir section
2.10.4 du sujet): ce is_marker est à 0 quand on est dans l'encodage des données brutes.
*/
void bitstream_write_nbits_eleve(struct bitstream_eleve *stream, uint32_t value, uint8_t nbits, bool is_marker);

/*
    Force l'exécution des écritures en attente sur le bitstream, s'il en
    existe.
*/
extern void bitstream_flush_eleve(struct bitstream_eleve *stream);

/*
    Détruit le bitstream passé en paramètre, en libérant la mémoire qui lui est
    associée.
*/
extern void bitstream_destroy_eleve(struct bitstream_eleve *stream);

#endif /* _bitstream_h_eleve_ */
