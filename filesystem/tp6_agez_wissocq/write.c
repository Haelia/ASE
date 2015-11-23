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
	unsigned int cylinder, sector;
	unsigned char buffer[BUFSIZE];

	if(argc !=3){
		printf("Format necessite 2 arguments: le cylindre et le secteur");
		exit(EXIT_FAILURE);
	}
	cylinder = atoi(argv[1]);
	sector = atoi(argv[2]);
	assert(init_hardware(HARDWARE_INI));
	for(i = 0; i < 15; i++)
		IRQVECTOR[i] = nothing;
	
	for (i = 0; i < BUFSIZE/4; i++) {
		buffer[4*i] = 0xDE;
		buffer[4*i+1] = 0xAD;
		buffer[4*i+2] = 0xBE;
		buffer[4*i+3] = 0xEF;
	}
	
	write_sector(cylinder,sector,buffer);
	exit(EXIT_SUCCESS);
}
