#ifndef recuperation
#define recuperation
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>


/*
  Recupere les argument mis en entree et propose une aide
  propose une sortie avec un nom "prore"
*/
void recuperation_argument(int argc, char const *argv[], char *entree, char *sortie, uint8_t *sample);


#endif /* recuperation */
