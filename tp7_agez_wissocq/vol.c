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

/* initialise le superbloc*/
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
  int res;
  if (superbloc.nb_free == 0){
    /*Plus de place pour créer un nouveau bloc*/
    return -1;
  }
  /*Récupère la position du premier bloc libre*/
  res = superbloc.first_free_bloc;
  struct freebloc_s fb;
  read_blocn(current_vol, res, (unsigned char *) &fb, sizeof(struct freebloc_s));
  superbloc.first_free_bloc = fb.next;
  fb.magic = !MAGIC_FREE;
  write_blocn(current_vol, res, (unsigned char *) &fb, sizeof(struct freebloc_s));
  superbloc.nb_free--;
  save_super();
  printf("Volume %d, bloc %d alloué.\n",current_vol, res );
  return res;
}

void free_bloc(unsigned int bloc){
  struct freebloc_s fb;
  if ((bloc >=0) && (bloc < disk_mbr->volumes[current_vol].size)) {
    read_blocn(current_vol, bloc, (unsigned char *) &fb, sizeof(struct freebloc_s));
    if (fb.magic != MAGIC_FREE) {
      unsigned int res_free = superbloc.first_free_bloc;
      fb.next = res_free;
      superbloc.first_free_bloc = bloc;
      superbloc.nb_free++;
      fb.magic = MAGIC_FREE;
      save_super();
      write_blocn(current_vol, bloc, (unsigned char *) &fb, sizeof(struct freebloc_s));
      printf("Volume %d, bloc %d libéré.\n",current_vol, bloc );
      return;
    }
    printf("Déjà libéré\n");
    return;
  }
  fprintf(stderr, "Le bloc que vous essayez de libérer est en dehors du volume\n");
  return;    
}

void read_inode(int vol, int bloc, inode_t *inode){
  read_bloc(vol, bloc, (unsigned char *) inode);
}

void write_inode(int vol, int bloc, inode_t *inode){
  write_bloc(vol, bloc, (unsigned char *)inode);
}

int create_inode(enum type_t type){
  inode_t inode;
  inode.type=type;
  memset(inode.direct,0,sizeof(inode.direct));
  inode.indirect=0;
  inode.double_indirect=0;
  inode.taille=0;
  int bloc =new_bloc();
  write_inode(current_vol, bloc,&inode);
  return bloc;
}

void delete_inode(int bloc){
  for(i=0;i<112;i++){
    free(inode.direct[i]);
  }
    
}
