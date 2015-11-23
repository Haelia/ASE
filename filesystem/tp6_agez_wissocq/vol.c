#include <stdlib.h>
#include "drive.h"
#include "vol.h"
#include "mbr.h"
#include "hw_config.h"

extern mbr_t *disk_mbr;

void to_physical(int vol, int bloc, int *cyl, int *sect) {
    *sect = disk_mbr->volumes[vol].start_sect + bloc;
    *cyl = disk_mbr->volumes[vol].start_cyl;
    
    
    while(*sect >= HDA_MAXSECTOR) {
        *sect -= HDA_MAXSECTOR;
        *cyl++;
    }
}




void read_bloc(const unsigned int vol, const unsigned int bloc, unsigned char* buffer) {
    int sector, cylinder;
    to_physical(vol, bloc, &cylinder, &sector);
    read_sector(cylinder, sector, buffer);
}

void write_bloc(const unsigned int vol, const unsigned int bloc, const unsigned char* buffer) {
    int sector, cylinder;
    to_physical(vol, bloc, &cylinder, &sector);
    write_sector(cylinder, sector, buffer);
}

void format_vol(const unsigned int vol) {
	int cyl, sect;
	to_physical(vol, 0, &cyl, &sect);
	format_sector(cyl, sect, 1, 0);
}
