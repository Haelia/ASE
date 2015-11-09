#ifndef _MBR_H_
#define _MBR_H_

#define MAX_VOLUMES 8
#define MAGIC 666

/* Structures */

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
    unsigned int magic;
	unsigned int nbVol;
	vol_t volumes[MAX_VOLUMES];
};
typedef struct mbr_s mbr_t;

/* Fonctions */

/**
 * Charge la MBR du disque
 */
void load_mbr();

void save_mbr();




#endif