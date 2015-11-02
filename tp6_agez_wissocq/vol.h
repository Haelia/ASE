#ifndef _VOL_H_
#define _VOL_H_
#include "drive.h"



/**
 * Convertit un numéro de bloc logique en secteur physique
 * @param vol Le numéro de volume
 * @param bloc Le numéro de bloc
 * @param cyl l'adresse de l'endroit ou le numéro de cylindre sera placé
 * @param sect l'adresse de l'endroit ou le numéro de secteur sera placé
 */
void to_physical(int vol, int bloc, int *cyl, int *sect);

void read_bloc(const unsigned int vol, const unsigned int bloc, unsigned char* buffer);

void write_bloc(const unsigned int vol, const unsigned int bloc, const unsigned char* buffer);

void format_vol(const unsigned int vol);






#endif