#include "down_sampler.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>


/*
  Effectue l'echantillonnage_horizontal si les facteur le permette (gerer dans le hello_word)
  Prend en parametre le tableau de mcu
*/
void echantillonnage_horizontal(struct mcu ma_mcu, uint8_t facteur)
{
    uint32_t* tableau_rgb = malloc(64*sizeof(uint32_t));
    for (uint8_t i=0; i<ma_mcu.v*ma_mcu.h/facteur; i++)
    {
        for (uint8_t m=0; m<facteur; m++)
        {
            for(uint8_t k=0;k<63;k = k+facteur)
            {
                uint32_t moy_bleu = 0;
                uint32_t moy_vert = 0;
                uint32_t moy_rouge = 0;
                uint32_t bleu;
                uint32_t vert;
                uint32_t rouge;
                //on va sommer facteur coefficients et en faire la moyenne
                for (uint8_t p =0; p<facteur; p++)
                {
                  bleu = ma_mcu.tableau_de_bloc[facteur*i+m].rgb[k+p] >> 16;
                  vert = (ma_mcu.tableau_de_bloc[facteur*i+m].rgb[k+p] - (bleu << 16)) >> 8;
                  rouge = ma_mcu.tableau_de_bloc[facteur*i+m].rgb[k+p] - bleu*(16*16*16*16)-vert*(16*16);
                  moy_bleu += bleu;
                  moy_vert += vert;
                  moy_rouge += rouge;
                }
                //On pondère la somme pour obtenir la moyenne
                moy_bleu = moy_bleu / facteur;
                moy_vert = moy_vert / facteur;
                moy_rouge = moy_rouge / facteur;

                //Calcul des nouveaux cb et cr
                uint8_t new_cb = -0.1687*moy_rouge - 0.3313*moy_vert + 0.5*moy_bleu +128;
                uint8_t new_cr = 0.5*moy_rouge - 0.4187*moy_vert - 0.0813*moy_bleu +128;

                //place dans le nouveau tableau
                uint8_t j;
                j = k - k%8 + (k%8)/facteur + 8*m/facteur;
                //On remplace
                ma_mcu.tableau_de_bloc[facteur*i].cb[j] = new_cb;
                ma_mcu.tableau_de_bloc[facteur*i].cr[j] = new_cr;

                tableau_rgb[j] = (moy_bleu << 16) + (moy_vert << 8) + moy_rouge;
            }
        }
        //copie dans les autres blocs
        for (uint8_t m=1; m<facteur; m++)
        {
          // if (ma_mcu.tableau_de_bloc[facteur*i+m].cb != NULL) {
          //   free(ma_mcu.tableau_de_bloc[facteur*i+m].cb);
          //   ma_mcu.tableau_de_bloc[facteur*i+m].cb = NULL;
          // }
          // if (ma_mcu.tableau_de_bloc[facteur*i+m].cr != NULL) {
          //   free(ma_mcu.tableau_de_bloc[facteur*i+m].cr);
          //   ma_mcu.tableau_de_bloc[facteur*i+m].cr = NULL;
          // }
          ma_mcu.tableau_de_bloc[facteur*i+m].cb = ma_mcu.tableau_de_bloc[facteur*i].cb;
          ma_mcu.tableau_de_bloc[facteur*i+m].cr = ma_mcu.tableau_de_bloc[facteur*i].cr;
        }
        for (size_t seum = 0; seum < facteur; seum++) {
          for (size_t k = 0; k < 64; k++) {
            ma_mcu.tableau_de_bloc[facteur*i+seum].rgb[k] = tableau_rgb[k];
          }
        }
    }
    free(tableau_rgb);
}
/*
  Effectue l'echantillonnage_vertical si les facteur le permette (gerer dans le hello_word)
  Prend en parametre le tableau de mcu
*/
void echantillonnage_vertical(struct mcu ma_mcu, uint8_t facteur, uint8_t facteur_horizontal)
{
    if(facteur_horizontal==ma_mcu.h && facteur_horizontal!=1)
    {
        facteur_horizontal = facteur_horizontal * facteur;
    }
    for (uint8_t i=0; i<(ma_mcu.v)*ma_mcu.h - ma_mcu.h*(facteur-1); i=i+facteur_horizontal)
    {
        for (uint8_t m=0; m<facteur; m++)
        {
            for(uint8_t k=0; k<63; k++)
            {
                uint32_t moy_bleu = 0;
                uint32_t moy_vert = 0;
                uint32_t moy_rouge = 0;
                uint32_t bleu;
                uint32_t vert;
                uint32_t rouge;
                //on va sommer facteur coefficients et en faire la moyenne
                for (uint8_t p =0; p<facteur; p++)
                {
                  bleu = ma_mcu.tableau_de_bloc[i+m*ma_mcu.h].rgb[k+p*8] >> 16;
                  vert = (ma_mcu.tableau_de_bloc[i+m*ma_mcu.h].rgb[k+p*8] - (bleu << 16)) >> 8;
                  rouge = ma_mcu.tableau_de_bloc[i+m*ma_mcu.h].rgb[k+p*8] - bleu*(16*16*16*16)-vert*(16*16);
                  moy_bleu += bleu;
                  moy_vert += vert;
                  moy_rouge += rouge;
                }
                //On pondère la somme pour obtenir la moyenne
                moy_bleu = moy_bleu / facteur;
                moy_vert = moy_vert / facteur;
                moy_rouge = moy_rouge / facteur;

                //Calcul des nouveaux cb et cr
                uint8_t new_cb = -0.1687*moy_rouge - 0.3313*moy_vert + 0.5*moy_bleu +128;
                uint8_t new_cr = 0.5*moy_rouge - 0.4187*moy_vert - 0.0813*moy_bleu +128;

                //place dans le nouveau tableau
                uint8_t j;
                j = (k-k%8)/facteur + k%8 + 64*m/facteur;;
                //On remplace
                ma_mcu.tableau_de_bloc[i].cb[j] = new_cb;
                ma_mcu.tableau_de_bloc[i].cr[j] = new_cr;

                if ((k+1)%8==0)
                {
                  k = k + 8*(facteur-1);
                }
            }
        }
        //copie dans les autres blocs
        for (uint8_t m=1; m<facteur; m++)
        {
          // if (ma_mcu.tableau_de_bloc[i+m*ma_mcu.h].cb != NULL) {
          //   free(ma_mcu.tableau_de_bloc[i+m*ma_mcu.h].cb);
          //   ma_mcu.tableau_de_bloc[i+m*ma_mcu.h].cb = NULL;
          // }
          // if (ma_mcu.tableau_de_bloc[i+m*ma_mcu.h].cr != NULL) {
          //   free(ma_mcu.tableau_de_bloc[i+m*ma_mcu.h].cr);
          //   ma_mcu.tableau_de_bloc[i+m*ma_mcu.h].cr = NULL;
          // }
          // free(ma_mcu.tableau_de_bloc[i+m*ma_mcu.h].cr);
          ma_mcu.tableau_de_bloc[i+m*ma_mcu.h].cb = ma_mcu.tableau_de_bloc[i].cb;
          ma_mcu.tableau_de_bloc[i+m*ma_mcu.h].cr = ma_mcu.tableau_de_bloc[i].cr;
        }
        if((i+1)%ma_mcu.h==0)
        {
            i+=ma_mcu.h*(facteur-1);
        }
    }
}
