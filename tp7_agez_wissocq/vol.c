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


unsigned int cylinder_of_bloc(unsigned int vol, unsigned int bloc) {
  assert(mbr.magic == MAGIC);
  assert(vol < mbr.nb_vol);
  assert(bloc < mbr.vol[vol].size);

  return mbr.vol[vol].cylinder + ((bloc + mbr.vol[vol].sector)/HDA_MAXSECTOR);
}

unsigned int sector_of_bloc(unsigned int vol, unsigned int bloc) {
  assert(disk_mbr.magic == MAGIC);
  assert(vol < disk_mbr.nb_vol);
  assert(bloc < disk_mbr.vol[vol].size);

  return ((bloc + disk_mbr.vol[vol].sector)%HDA_MAXSECTOR);
}

void read_blocn(unsigned int vol, unsigned int bloc, unsigned char *buffer, unsigned int bufsize) {
  read_sectorn(cylinder_of_bloc(vol,bloc), sector_of_bloc(vol,bloc), buffer, bufsize);
}

void write_blocn(unsigned int vol, unsigned int bloc, unsigned char *buffer, unsigned int bufsize) {
  write_sectorn(cylinder_of_bloc(vol,bloc), sector_of_bloc(vol,bloc), buffer, bufsize);
}

void load_super(unsigned int vol) {
  read_blocn(vol,0,(unsigned char*) &superbloc,sizeof(struct superbloc_s));
  current_vol = vol;
  return;
}

void init_super(unsigned int vol, char* name) {
  int i;
  load_super(vol);
  if (superbloc.magic == MAGIC_SB) {
    fprintf(stderr, "Superbloc déjà initialisé.\n");
    exit(EXIT_FAILURE);
  }
  else {
    int l;
    superbloc.magic = MAGIC_SB;
    strncpy(superbloc.name, name, MAX_TAILLE);
    superbloc.first_free_bloc = 1;
    superbloc.root = 0;
    superbloc.nb_free = mbr.vol[vol].size-1;
    save_super(vol);
    for (i = 1, l = mbr.vol[vol].size; i < l; i++) {
      struct free_bloc_s fb;
      fb.next = (i+1)%mbr.vol[vol].size;
      fb.magic = MAGIC_FREE;
      write_blocn(vol, i, (unsigned char *) &fb, sizeof(struct free_bloc_s));
    }
  }
}
