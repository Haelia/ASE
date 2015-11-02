#include "mbr.h"
#include "drive.h"
#include "hardware.h"
#include "hw_config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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


int main(int argc, char** argv) {
    int i;
    /* initialisation hardware */
    if(init_hardware(HARDWARE_INI) == 0) {
            fprintf(stderr, "Erreur lors de l'initialisation du hardware\n");
            exit(EXIT_FAILURE);
    }
    for(i = 0; i < 15; i++)
            IRQVECTOR[i] = nothing;
    
    disk_mbr = (mbr_t *) malloc(sizeof(mbr_t)); 
    
    disk_mbr->magic = atoi(argv[1]);
    save_mbr();
    load_mbr();
    printf("%d\n", disk_mbr->magic);
    free(disk_mbr);
    return 0;
}






