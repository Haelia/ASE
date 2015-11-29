#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "drive.h"
#include "vol.h"
#include "mbr.h"
#include "hw_config.h"
#include <assert.h>

#define NB_DIRECT (256 - 5*sizeof(int))/sizeof(int)
#define NB_INDIRECT 256/sizeof(int)
#define NB_TOTAL (NB_DIRECT + NB_INDIRECT + NB_INDIRECT*NB_INDIRECT)

extern mbr_t *disk_mbr;

void to_physical(int vol, int bloc, int *cyl, int *sect) {
    *sect = disk_mbr->volumes[vol].start_sect + bloc;
    *cyl = disk_mbr->volumes[vol].start_cyl;
    
        
    while(*sect >= HDA_MAXSECTOR) {
        *sect -= HDA_MAXSECTOR;
        *cyl += 1;
    }
}

/* ***************************
 * Lecture écriture de blocs *
 * ***************************/

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


/* ***********
 * Superbloc *
 * ***********/

void load_super(unsigned int vol) {
    read_blocn(vol, 0, (unsigned char*) &superbloc, sizeof(struct superbloc_s));
    current_vol = vol;
    return;
}

void save_super() {
    write_blocn(current_vol, 0, (unsigned char *) &superbloc, sizeof(struct superbloc_s));
}

/** initialise le superbloc */
void init_super(unsigned int vol, char* name) {
    int i, size;
    load_super(vol);
    if (superbloc.magic == MAGIC_SB) {
        printf("Superbloc déjà initialisé.\n");
        exit(EXIT_FAILURE);
    }
    else {
        superbloc.magic = MAGIC_SB;
        strncpy(superbloc.name, name, MAX_TAILLE);
        superbloc.first_free_bloc = 1;
        superbloc.root = 0;
        superbloc.nb_free = disk_mbr->volumes[vol].size-1;
        save_super(vol);
        for (i = 1, size = disk_mbr->volumes[vol].size; i < size; i++) {
			struct freebloc_s fb;
			fb.next = (i+1) % size;
			fb.magic = MAGIC_FREE;
			write_blocn(vol, i, (unsigned char *) &fb, sizeof(struct freebloc_s));
        }
    }
}


/* ***********************
 * Manipulation de blocs *
 * ***********************/

unsigned int new_bloc(){
    int res;
    struct freebloc_s fb;
    if (superbloc.nb_free == 0){
    /* Plus de place pour créer un nouveau bloc */
        return -1;
    }
    /* Récupère la position du premier bloc libre */
    res = superbloc.first_free_bloc;
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
    int bloc =new_bloc();
    inode_t inode;
    inode.type=type;
    memset(inode.direct,0,sizeof(inode.direct));
    inode.indirect=0;
    inode.double_indirect=0;
    inode.taille=0;
    write_inode(current_vol, bloc,&inode);
    return bloc;
}

int delete_inode (unsigned int inumber) {
  int i,j;
  struct inode_s inode;
  unsigned char buf1[256];
  unsigned char buf2[256];

  read_inode(current_vol, inumber,&inode);
  // direct blocks
  for (i = 0; i < NB_DIRECT; i++){
    if (inode.direct[i]) {
      free_bloc(inode.direct[i]);
    }
  }

  // indirect blocks
  if (inode.indirect) {
    read_bloc(current_vol, inode.indirect, buf1);
    for (i = 0; i < NB_INDIRECT; i++) {
      if (buf1[i]) {
        free_bloc(buf1[i]);
      }
    }
    free_bloc(inode.indirect);
  }

  // indirect_double blocks
  if (inode.double_indirect) {
    read_bloc(current_vol, inode.double_indirect, buf1);
    for (i = 0; i < NB_INDIRECT; i++) {
      if (buf1[i]) {
        read_bloc(current_vol, buf1[i], buf2);
        for (j = 0; j < NB_INDIRECT; i++) {
          if (buf2[j]) {
            free_bloc(buf2[j]);
          }
        }
        free_bloc(buf1[i]);
      }
    }
    free_bloc(inode.double_indirect);
  }

  free_bloc(inumber);
  return 1;
}

unsigned int read_indirect(unsigned int ind, unsigned int fbloc, int do_allocate) {
  int entries_direct[256];
  read_bloc(current_vol, ind, (unsigned char *) entries_direct);
  if (entries_direct[fbloc] == 0 && do_allocate) {
    entries_direct[fbloc] = new_bloc();
    if (entries_direct[fbloc] == -1) {
      fprintf(stderr, "Error: Impossible to allocate a new bloc, out of memory.\n");
      return 0;
    }
    write_bloc(current_vol, ind, (unsigned char *) entries_direct);
  }
  return entries_direct[fbloc];
}

unsigned int read_indirect_double(unsigned int ind_double, unsigned int fbloc, int do_allocate) {
  int entries_indirect[256];
  unsigned int res;
  // load first indirection in entries_indirect
  read_bloc(current_vol, ind_double, (unsigned char *) entries_indirect);
  if (entries_indirect[fbloc/NB_INDIRECT] == 0) {
    if (!do_allocate){
      return 0;
    }
    else {
      entries_indirect[fbloc/NB_INDIRECT] = new_bloc();
      if (entries_indirect[fbloc/NB_INDIRECT] == - 1) {
        fprintf(stderr, "Error: Impossible to allocate a new bloc, out of memory.\n");
        return 0;
      }
      write_bloc(current_vol, ind_double, (unsigned char *) entries_indirect);
    }
  }
  // load second indirection in entries_indirect
  res = entries_indirect[fbloc/NB_INDIRECT];
  read_bloc(current_vol, res, (unsigned char *) entries_indirect);
  if (entries_indirect[fbloc%NB_INDIRECT] == 0) {
    if (!do_allocate) {
      return 0;
    }
    else {
      entries_indirect[fbloc%NB_INDIRECT] = new_bloc();
      if (entries_indirect[fbloc%NB_INDIRECT] == - 1) {
        fprintf(stderr, "Error: Impossible to allocate a new bloc, out of memory.\n");
        return 0;
      }
      write_bloc(current_vol, res, (unsigned char *) entries_indirect);
    }
  }
  return entries_indirect[fbloc%NB_INDIRECT];
}

unsigned int vbloc_of_fbloc(unsigned int inumber, unsigned int fbloc, int do_allocate) {
  inode_t inode;
  read_inode(current_vol, inumber, &inode);
  if (fbloc < NB_DIRECT) { // searching in direct entries
    if ((inode.direct[fbloc] == 0) && do_allocate) {
      inode.direct[fbloc] = new_bloc();
      if (inode.direct[fbloc] == -1) {
        fprintf(stderr, "Error: Impossible to allocate a new bloc, out of memory.\n");
        return 0;
      }
      write_inode(current_vol, inumber, &inode);
    }
    return inode.direct[fbloc];
  }
  else if (fbloc < (NB_DIRECT + NB_INDIRECT)) { // searching in indirect entries
    if (inode.indirect == 0) {
      if (!do_allocate){
        return 0;
      }
      else {
        inode.indirect = new_bloc();
        if (inode.double_indirect == -1) {
          fprintf(stderr, "Error: Impossible to allocate a new bloc, out of memory.\n");
          return 0;
        }
        write_inode(current_vol, inumber, &inode);
      }
    }
    return read_indirect(inode.indirect, fbloc - NB_DIRECT,do_allocate);
  }
  else if (fbloc < NB_TOTAL) { // searching in indirect double entries
    if (inode.double_indirect == 0) {
      if (!do_allocate){
        return 0;
      }
      else {
        inode.double_indirect = new_bloc();
        if (inode.double_indirect == -1) {
          fprintf(stderr, "Error: Impossible to allocate a new bloc, out of memory.\n");
          return 0;
        }
        write_inode(current_vol, inumber, &inode);
      }
    }
    return read_indirect_double(inode.double_indirect, fbloc-(NB_DIRECT+NB_INDIRECT), do_allocate);
  }
  return 0;
}
