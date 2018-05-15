#ifndef decoupage_mcu
#define decoupage_mcu

#include <stdint.h>

/*structure representant le mcu*/

struct mcu;



struct mcu *decoupage_mcu(const char *ppm_filename, int8_t h1, int8_t v1);

/* permet de recuperer les échantillons RGB à partir du fichier
 * d’entrée PPM premiere case type du tableau deuxieme case largeur troisieme case
 * hauteur */
int8_t *recuperation_rgb(const char *ppm_filename);

/*retourne POUR L'INSTANT QU'UN TABLEAU DE 64 */
int *tableau_decoupe(*tab_rgb);

/**/
struct mcu *renvoie_mcu_8_8(*tableau_decoupe);


#endif /* decoupage_mcu */
