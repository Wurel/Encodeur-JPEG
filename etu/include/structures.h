#ifndef structures
#define structures

struct bloc{
  uint32_t *rgb;
  uint8_t *y;
  uint8_t *cb;
  uint8_t *cr;
};

struct mcu{
  uint8_t h;
  uint8_t v;
  struct bloc *tableau_de_bloc;
};
struct bloc_apres_dct{
    int16_t *y;
    int16_t *cb;
    int16_t *cr;
};



#endif /* structures */
