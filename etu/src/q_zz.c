#include "q_zz.h"


void quantification_composante(int8_t *composante) {
  for (uint8_t i = 0; i < 64; i++) {
    composante[i] = composante[i]/compressed_Y_table[i];
  }
}
