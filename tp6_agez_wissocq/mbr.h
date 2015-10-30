#ifndef _MBR_H_
#define _MBR_H_

#define MAX_VOLUMES 8

//Structures

/**
 * Définit les différents types de volumes
 */
enum vol_type {
	BASE, SECONDARY, OTHER
};

/**
 * Définit un volume par son emplacement, sa taille et son type
 */
struct vol_s {
	unsigned int start_cyl;
	unsigned int start_sect;
	unsigned int size;
	enum vol_type type;
};
typedef struct vol_s vol_t;

/**
 * Définit la MBR du disque, contenant la liste des volumes
 */
struct mbr_s {
	unsigned short int nbVol;
	vol_t volumes[MAX_VOLUMES];
};
typedef struct mbr_s mbr_t;

// Fonctions

/**
 * Charge la MBR du disque
 * @param mbr La structure à remplir avec les données de la MBR
 */
void load_mbr(mbr_t mbr);

/**
 * Convertit un numéro de bloc logique en secteur physique
 * @param vol Le numéro de volume
 * @param bloc Le numéro de bloc
 * @param cyl l'adresse de l'endroit ou le numéro de cylindre sera placé
 * @param sect l'adresse de l'endroit ou le numéro de secteur sera placé
 */
void to_physical(int vol, int bloc, int *cyl, int *sect);


#endif