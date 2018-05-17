#include "rgb2ycbcr.h"

// transforme UNE mcu (son pointeur est mis en parametre) en RGB en une mcu en YCbCr
void transformation_bloc_rgb_ycbcr(struct bloc *blo)
{
  //pour chaque tableau de composante de mcu ici juste un bloc pour la mcu
  for (size_t i = 0; i < 8; i++) {
    for (size_t j = 0; j < 8; j++) {
      uint8_t rouge = blo->rgb[j+8*i] >> 16;
      uint32_t vert = (blo->rgb[j+8*i]- (rouge << 16)) >> 8;
      uint32_t bleu = blo->rgb[j+8*i]-rouge*(16*16*16*16)-vert*(16*16);
      blo->y[j+8*i] = 0.299*rouge + 0.587*vert + 0.114*bleu;
      blo->cb[j+8*i] = -0.1687*rouge - 0.3313*vert + 0.5*bleu +128;
      blo->cr[j+8*i] = 0.5*rouge + 0.4187*vert - 0.0813*bleu +128;
    }
  }
}
