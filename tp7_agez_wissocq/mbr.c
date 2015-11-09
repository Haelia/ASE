#include "mbr.h"
#include "drive.h"
#include "hardware.h"
#include "hw_config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vol.h"

#define BUFSIZE 256

/* GLOBAL MBR */
mbr_t *disk_mbr;


void nothing() {
}


void load_mbr() {
    unsigned char buf[BUFSIZE];
    read_sector(0, 0, buf);
    memcpy((void *) disk_mbr, (void *) buf, sizeof(mbr_t));
}

void save_mbr() {
    unsigned char buf[BUFSIZE];
    memcpy((void *) buf, (void *) disk_mbr, sizeof(mbr_t));
    write_sector(0, 0, buf);
}

/* Permet de  vérifier que ça fonctionne
int main(int argc, char** argv) {
    int i;
	unsigned char buf[BUFSIZE];
    if(init_hardware(HARDWARE_INI) == 0) {
            fprintf(stderr, "Erreur lors de l'initialisation du hardware\n");
            exit(EXIT_FAILURE);
    }
    for(i = 0; i < 15; i++)
            IRQVECTOR[i] = nothing;
    disk_mbr = (mbr_t *) malloc(sizeof(mbr_t)); 
	
	disk_mbr->magic = MAGIC;
	disk_mbr->nbVol = 2;
	disk_mbr->volumes[0].start_cyl = 0;
	disk_mbr->volumes[0].start_sect = 1;
	disk_mbr->volumes[0].size = 10;
	
	disk_mbr->volumes[1].start_cyl = 0;
	disk_mbr->volumes[1].start_sect = 11;
	disk_mbr->volumes[1].size = 128;
	
	save_mbr();
	
	for (i = 0; i < BUFSIZE/4; i++) {
		buf[4*i]   = 0xDE;
		buf[4*i+1] = 0xAD;
		buf[4*i+2] = 0xBE;
		buf[4*i+3] = 0xEF;
	}
	write_bloc(0, 0, buf);
	write_bloc(1, 0, buf);
	write_bloc(1, 5, buf);
	
    free(disk_mbr);
    return 0;
}
*/