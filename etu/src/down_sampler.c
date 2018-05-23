// #include "down_sampler.h"
// #include <stdlib.h>
// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
// #include <math.h>
//
// <<<<<<< HEAD
// void echantillonage_horizontal(struct mcu)
// {
//     for (i=0; i<mcu.v; i++)
//     {
//         struct bloc bloc1 = mcu.tableau_de_bloc[2*i];
//         struct bloc bloc2 = mcu.tableau_de_bloc[2*i+1];
//         for(uint8_t k=0;k<63;k+2)
//         {
//             //1er bloc
//             uint8_t bleu1 = bloc1.rgb[k] >> 16;
//             uint32_t vert1 = (bloc1.rgb[k]- (bleu1 << 16)) >> 8;
//             uint32_t rouge1 = bloc1.rgb[k]-bleu1*(16*16*16*16)-vert1*(16*16);
//
//             uint8_t bleu2 = bloc1.rgb[k+1] >> 16;
//             uint32_t vert2 = (bloc1.rgb[k+1]- (bleu2 << 16)) >> 8;
//             uint32_t rouge2 = bloc1.rgb[k+1]-bleu2*(16*16*16*16)-vert2*(16*16);
//
//             //Moyenne
//             uint8_t moy_bleu = (bleu1+bleu2)/2;
//             uint8_t moy_touge = (rouge1+rouge2)/2;
//             uint8_t moy_vert = (vert1+vert2)/2;
//
//             //Calcul des nouveaux cb et cr
//             uint8_t new_cb = -0.1687*moy_rouge - 0.3313*moy_vert + 0.5*moy_bleu +128;
//             uint8_t new_cr = 0.5*moy_rouge - 0.4187*moy_vert - 0.0813*moy_bleu +128;
//
//             //place dans le nouveau tableau
//             uint8_t j;
//             j = k - k%8;
//             //On remplace
//             bloc1.cb[j] = new_cb;
//             bloc1.cr[j] = new_cr;
//
//             //Deuxieme bloc
//
//             bleu1 = bloc2.rgb[k] >> 16;
//             vert1 = (bloc2.rgb[k]- (bleu1 << 16)) >> 8;
//             rouge1 = bloc2.rgb[k]-bleu1*(16*16*16*16)-vert1*(16*16);
//
//             bleu2 = bloc2.rgb[k+1] >> 16;
//             vert2 = (bloc2.rgb[k+1]- (bleu2 << 16)) >> 8;
//             rouge2 = bloc2.rgb[k+1]-bleu2*(16*16*16*16)-vert2*(16*16);
//
//             //Moyenne
//             moy_bleu = (bleu1+bleu2)/2;
//             moy_touge = (rouge1+rouge2)/2;
//             moy_vert = (vert1+vert2)/2;
//
//             //Calcul des nouveaux cb et cr
//             new_cb = -0.1687*moy_rouge - 0.3313*moy_vert + 0.5*moy_bleu +128;
//             new_cr = 0.5*moy_rouge - 0.4187*moy_vert - 0.0813*moy_bleu +128;
//
//             //place dans le nouveau tableau
//             j = k - k%8 + 4;
//
//             //On remplace
//             bloc1.cb[j] = new_cb;
//             bloc1.cr[j] = new_cr;
//
//             //copie dans le deuxieme bloc
//             bloc2.cb = bloc1.cb;
//             bloc2.cr = bloc1.cr;
//         }
//     }
// }
// =======
// // void echantillonage_horizontal(struct mcu)
// // {
// //     for (i=0; i<mcu.v; i++)
// //     {
// //         struct bloc bloc1 = mcu.tableau_de_bloc[2*i];
// //         struct bloc bloc2 = mcu.tableau_de_bloc[2*i+1];
// //         for(uint8_t k=0;k<63;k+2)
// //         {
// //             uint8_t bleu1 = bloc1.rgb[k] >> 16;
// //             uint32_t vert1 = (bloc1.rgb[k]- (bleu << 16)) >> 8;
// //             uint32_t rouge1 = bloc1.rgb[k]-bleu*(16*16*16*16)-vert*(16*16);
// //
// //             uint8_t bleu2 = bloc1.rgb[k+1] >> 16;
// //             uint32_t vert2 = (bloc1.rgb[k+1]- (bleu << 16)) >> 8;
// //             uint32_t rouge2 = bloc1.rgb[k+1]-bleu*(16*16*16*16)-vert*(16*16);
// //
// //
// //             uint8_t j=0;
// //             j = k - k%8;
// //             //On remplace
// //             bloc1.cb[j] = new_cb;
// //             bloc1.cb[j] = new_cb;
// //             bloc2.cr[j] = new_cr;
// //             bloc2.cr[j] = new_cr;
// //         }
// //     }
// // }
// >>>>>>> d66f12a4d29b8e0ee3193dc264fc8a699f53b71d
