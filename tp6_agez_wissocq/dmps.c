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

void toHexa(unsigned char * buffer) {
	int i,j;
	for (i = 0; i < BUFSIZE/16; i++) {
		printf("%04x :",i*16);
		for (j = 0; j < 16; j++) {
			if (j%4 == 0)
				printf("  ");
			printf(" %02x",buffer[i*16+j]);
		}
		printf("\n");
	}
	return;
}


int main (int argc, char ** argv) {
	int i;
	unsigned int cylinder, sector;
	unsigned char buffer[BUFSIZE];
	
	if(argc !=3){
		printf("Format necessite 2 arguments: le cylindre et le secteur\n");
		exit(EXIT_FAILURE);
	}

	
	
	cylinder = atoi(argv[1]);
	sector = atoi(argv[2]);
	
	
	/* initialisation hardware */
	if(init_hardware(HARDWARE_INI) == 0) {
		fprintf(stderr, "Erreur lors de l'initialisation du hardware\n");
		exit(EXIT_FAILURE);
	}
	for(i = 0; i < 15; i++)
		IRQVECTOR[i] = nothing;
	
	
	read_sector(cylinder,sector,buffer);
	toHexa(buffer);
	exit(EXIT_SUCCESS);
}
