#include "down_sampler.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

// void echantillonage_horizontal(struct mcu)
// {
//     for (i=0; i<mcu.v; i++)
//     {
//         struct bloc bloc1 = mcu.tableau_de_bloc[2*i];
//         struct bloc bloc2 = mcu.tableau_de_bloc[2*i+1];
//         for(uint8_t k=0;k<63;k+2)
//         {
//             uint8_t bleu1 = bloc1.rgb[k] >> 16;
//             uint32_t vert1 = (bloc1.rgb[k]- (bleu << 16)) >> 8;
//             uint32_t rouge1 = bloc1.rgb[k]-bleu*(16*16*16*16)-vert*(16*16);
//
//             uint8_t bleu2 = bloc1.rgb[k+1] >> 16;
//             uint32_t vert2 = (bloc1.rgb[k+1]- (bleu << 16)) >> 8;
//             uint32_t rouge2 = bloc1.rgb[k+1]-bleu*(16*16*16*16)-vert*(16*16);
//
//
//             uint8_t j=0;
//             j = k - k%8;
//             //On remplace
//             bloc1.cb[j] = new_cb;
//             bloc1.cb[j] = new_cb;
//             bloc2.cr[j] = new_cr;
//             bloc2.cr[j] = new_cr;
//         }
//     }
// }
