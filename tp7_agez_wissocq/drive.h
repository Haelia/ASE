#ifndef _DRIVE_H_
#define _DRIVE_H_

/**
 * Permet de déplacer la tête de lecture
 * @param cylinder Le numéro de cylindre
 * @param sector Le numéro de secteur
 */
void seek (const unsigned int cylinder, const unsigned int sector);

/**
 * Lit le secteur et place le résultat dans buffer
 * @param cylinder Le numéro de cylindre
 * @param sector Le numéro de secteur
 * @param buffer Le buffer dans lequel le contenu du secteur doit être placé
 */
void read_sector(const unsigned int cylinder, const unsigned int sector, unsigned char *buffer);

/**
 * Ecrit le contenu du buffer sur le secteur
 * @param cylinder Le numéro de cylindre
 * @param sector Le numéro de secteur
 * @param buffer Le buffer dans lequel le contenu est lu
 */
void write_sector(const unsigned int cylinder, const unsigned int sector, const unsigned char *buffer);

/**
 * Formate un nombre défini de secteurs. 
 * Cette fonction peut formater plusieurs secteurs sur un même cylindre,
 * mais elle doit être appelée plusieurs fois si les secteurs à formater sont dispersé sur plusieurs secteurs
 * @param cylinder Le numéro de cylindre
 * @param sector Le numéro de secteur
 * @param nsector Le nombre de secteurs à formater
 * @param value La valeur à écrire pour formater le(s) secteur(s)
 */
void format_sector(const unsigned int cylinder, const unsigned int sector, const unsigned int nsector, const unsigned int value);


void read_sectorn(unsigned int cylinder,unsigned int sector, unsigned char *buffer, unsigned int bufsize);
void write_sectorn(unsigned int cylinder, unsigned int sector, const unsigned char *buffer, unsigned int bufsize);

#endif