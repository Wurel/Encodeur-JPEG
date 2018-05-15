#ifndef decoupage_mcu
#define decoupage_mcu

#include <stdint.h>

/*structure representant le mcu*/

struct mcu;


/*permet de recuperer les échantillons RGB à partir du fichier
d’entrée PPM premiere case type du tableau deuxieme case largeur troisieme case
hauteur*/
int8_t *recuperation_rgb(const char *ppm_filename);


#endif /* decoupage_mcu */
