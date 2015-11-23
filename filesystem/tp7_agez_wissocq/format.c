#include <stdio.h>
#include <stdlib.h>
#include "drive.h"
#include "hardware.h"
#include "hw_config.h"
#include <assert.h>

#define BUFSIZE HDA_SECTORSIZE

/* Fonction permettant de remplacer les interruptions */
static void nothing(){
	return;
}


int main (int argc, char ** argv) {
	int i;
	
	if(argc != 1){
		printf("format ne prend pas d'argument. Il écrit la valeur 0 sur tout le disque");
		exit(EXIT_FAILURE);
	}

	if (init_hardware(HARDWARE_INI) == 0) {
		printf("Erreur lors de l'initialisation du hardware");
		exit(EXIT_FAILURE);
	}
	
	for(i = 0; i < 15; i++)
		IRQVECTOR[i] = nothing;
	for (i = 0; i < HDA_MAXCYLINDER; i++)
		format_sector(i, 0, HDA_MAXSECTOR, 0);
	exit(EXIT_SUCCESS);
}
