#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "drive.h"
#include "vol.h"
#include "mbr.h"
#include "hw_config.h"
#include <assert.h>

extern mbr_t *disk_mbr;

void to_physical(int vol, int bloc, int *cyl, int *sect) {
    *sect = disk_mbr->volumes[vol].start_sect + bloc;
    *cyl = disk_mbr->volumes[vol].start_cyl;
    
    
    while(*sect >= HDA_MAXSECTOR) {
        *sect -= HDA_MAXSECTOR;
        *cyl += 1;
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
  assert(disk_mbr->magic == MAGIC);
  assert(vol < disk_mbr->nbVol);
  assert(bloc < disk_mbr->volumes[vol].size);

  return disk_mbr->volumes[vol].start_cyl + ((bloc + disk_mbr->volumes[vol].start_sect)/HDA_MAXSECTOR);
}

unsigned int sector_of_bloc(unsigned int vol, unsigned int bloc) {
  assert(disk_mbr->magic == MAGIC);
  assert(vol < disk_mbr->nbVol);
  assert(bloc < disk_mbr->volumes[vol].size);

  return ((bloc + disk_mbr->volumes[vol].start_sect)%HDA_MAXSECTOR);
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



void save_super() {
  write_blocn(current_vol,0,(unsigned char *) &superbloc, sizeof(struct superbloc_s));
}



void init_super(unsigned int vol, char* name) {
  int i;
  load_super(vol);
  if (superbloc.magic == MAGIC_SB) {
    printf("Superbloc déjà initialisé.\n");
    exit(EXIT_FAILURE);
  }
  else {
    int l;
    superbloc.magic = MAGIC_SB;
    strncpy(superbloc.name, name, MAX_TAILLE);
    superbloc.first_free_bloc = 1;
    superbloc.root = 0;
    superbloc.nb_free = disk_mbr->volumes[vol].size-1;
    save_super(vol);
    for (i = 1, l = disk_mbr->volumes[vol].size; i < l; i++) {
      struct freebloc_s fb;
      fb.next = (i+1)%disk_mbr->volumes[vol].size;
      fb.magic = MAGIC_FREE;
      write_blocn(vol, i, (unsigned char *) &fb, sizeof(struct freebloc_s));
    }
  }
}

unsigned int new_bloc(){
return 0;
}

void free_bloc(unsigned int bloc){
    
}


void read_inode(int vol, int bloc, inode_t *inode){
       read_bloc(vol, bloc, (char *) inode);
}

void write_inode(int vol, int bloc, inode_t *inode){
       write_bloc(vol, bloc, (char *)inode);
}

int create_inode(enum type_t type){
        inode_t inode;
        inode.type=type;
        memset(inode.direct,0,sizeof(inode.direct));
        inode.indirect=0;
        inode.double_indirect=0;
        inode.taille=0;
        int bloc =new_bloc();
        write_inode(bloc,&inode);
        return bloc;
}

void delete_inode(int bloc){
    for(i=0;i<112;i++){
        free(inode.direct[i]);
    }
    
}