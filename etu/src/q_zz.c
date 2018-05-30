#include "q_zz.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/*
  Renvoie un tableau passé en paramètre réordonné en zigzag dans un autre tableau
*/
int16_t* zigzag_composante(int16_t *composante)
{
    int16_t* zigzag = malloc(64 * sizeof(int16_t));
    int16_t i = 0; //Parcours la composante originale
    int16_t j = 0; //Ecrit dans le nouveau tableau zigzag
    int16_t deuxieme_partie = 0;
    while(j < 64)
    {
        //L'algorithme agit de deux maniéres différentes, que l'on soit dans la partie haute gauche, ou dans la partie basse droite.
        if (i==7) //Lorsque i=7, on etre dans la partie basse droite
        {
            deuxieme_partie = 1;
        }
        zigzag[j] = composante[i];
        int16_t diagonale = (i % 7);
        if (deuxieme_partie == 1)
        {
            diagonale += 7;
        }
        if (diagonale % 2 == 1)
        {
            if(deuxieme_partie == 0)
            {
                if (diagonale * (7 + 1) >  i)
                {
                    i += 7;
                }
                else
                {
                    i += 8;
                }
            }
            else
            {
                if (diagonale + 7 * 7 > i)
                {
                    i += 7;
                }
                else
                {
                    i += 1;
                }
            }
        }
        else
        {
            if (deuxieme_partie == 0)
            {
                if (i > diagonale)
                {
                    i -= 7;
                }
                else
                {
                    i += 1;
                }
            }
            else
            {
                if (i > 8*diagonale - 49)
                {
                    i -= 7;
                }
                else
                {
                    i += 8;
                }
            }
        }
        j++;
    }
    free(composante);
    composante = NULL;
    return zigzag;
}

/*
  Fait le calcul de quantification pour Y :
  Divise terme à terme le tableau de la composante, par la table de quantification
*/
void quantification_Y(int16_t *composante)
{
  for (uint16_t i = 0; i < 64; i++)
  {
    composante[i] = composante[i]/compressed_Y_table[i];
  }
}

/*
  Fait le calcul de quantification pour Cb ou Cr :
  Divise terme à terme le tableau de la composante, par la table de quantification
*/
void quantification_Cb_Cr(int16_t *composante)
{
  for (uint16_t i = 0; i < 64; i++)
  {
    composante[i] = composante[i]/compressed_CbCr_table[i];
  }
}


/*-----------------------------------TEST-----------------------------------*/

// int main(int argc, char const *argv[])
// {
//     //TEST DU ZIGZAG
//     int16_t* test_zigzag = malloc(64*sizeof(int16_t));
//     for(int16_t i = 0; i < 8; i++)
//     {
//         for(int16_t j = 0; j < 8; j++)
//         {
//             test_zigzag[8*i+j]=i;
//         }
//     }
//     for (size_t i = 0; i < 64; i++)
//     {
//         printf("%d\n", test_zigzag[i]);
//     }
//     printf("Le tableau en zig-zag : \n");
//     int16_t* apres_zigzag = zigzag_composante(test_zigzag);
//     for (size_t i = 0; i < 64; i++)
//     {
//         printf("%d\n", apres_zigzag[i]);
//     }
//     //FIN DU TEST DU ZIG ZAG
//     return 0;
// }
