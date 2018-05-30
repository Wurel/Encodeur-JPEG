#ifndef structures
#define structures

#include <stdint.h>

/*
   Notre structure de bloc contenant:
   - un tableau des composantes en rgb (1D)
   ( avec bleu sur l'octet de poids fort, vert le suivant et rouge sur l'octet de poids faible )
   - un tableau pour chaque composante Y Cb et Cr (1D)
*/
struct bloc{
  uint32_t *rgb;
  uint8_t *y;
  uint8_t *cb;
  uint8_t *cr;
};

/*
  Notre structure de MCU
  - h et v correspondent au nb de bloc sur l'horizontale et la vertical
  (dans le cas de base h = v = 1 donc une MCU correspond exactement à un bloc)
  - le tableau de bloc en 1D
  - le tableau de bloc_apres_dct
*/
struct mcu{
  uint8_t h;
  uint8_t v;
  struct bloc *tableau_de_bloc;
  struct bloc_apres_dct *tableau_de_bloc_apres_dct;
};

/*
  Meme structure que precedement (bloc) qui contiendra les coefficent du bloc apres dct
  le pointeur sur le tableau rgb n'est plus utile a cette etape
*/
struct bloc_apres_dct{
    int16_t *y;
    int16_t *cb;
    int16_t *cr;
};


#endif /* structures */
