#include "q_zz.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

int8_t* zigzag_composante(int8_t *composante)
{
    int8_t* zigzag = malloc(64 * sizeof(int8_t));
    int8_t i = 0;
    int8_t j = 0;
    int8_t deuxieme_partie = 0;
    while(j < 64)
    {
        if (i==7)
        {
            deuxieme_partie = 1;
        }
        zigzag[j] = composante[i];
        int8_t diagonale = (i % 7);
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
    return zigzag;
}

void quantification_composante(int8_t *composante)
{
  for (uint8_t i = 0; i < 64; i++)
  {
    composante[i] = composante[i]/compressed_Y_table[i];
  }
}


// int main(int argc, char const *argv[])
// {
//     //TEST DU ZIGZAG
//     int8_t* test_zigzag = malloc(64*sizeof(int8_t));
//     for(int8_t i = 0; i < 8; i++)
//     {
//         for(int8_t j = 0; j < 8; j++)
//         {
//             test_zigzag[8*i+j]=i;
//         }
//     }
//     for (size_t i = 0; i < 64; i++)
//     {
//         printf("%d\n", test_zigzag[i]);
//     }
//     printf("Le tableau en zig-zag : \n");
//     int8_t* apres_zigzag = zigzag_composante(test_zigzag);
//     for (size_t i = 0; i < 64; i++)
//     {
//         printf("%d\n", apres_zigzag[i]);
//     }
//     //FIN DU TEST DU ZIG ZAG
//     return 0;
// }
