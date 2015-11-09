#ifndef _VOL_H_
#define _VOL_H_
#include "drive.h"
#define MAGIC_SB 58
#define MAGIC_FREE 33
#define MAX_TAILLE 32

unsigned int current_vol;
struct superbloc_s superbloc;

struct superbloc_s {
  int magic;
  unsigned int serialn;
  char name[MAX_TAILLE];
  unsigned int first_free_bloc;
  unsigned int root;
  int nb_free;
};

struct freebloc_s {
  unsigned int next;
  int magic;
};

enum type_t {DIR, FICHIER};

struct inode_s{
enum type_t type;
int direct[112];
int indirect;
int double_indirect;
int taille;    
};

typedef struct inode_s inode_t;
typedef int indir_t[128];
typedef int dblindir_t[128];

/**
 * Convertit un numéro de bloc logique en secteur physique
 * @param vol Le numéro de volume
 * @param bloc Le numéro de bloc
 * @param cyl l'adresse de l'endroit ou le numéro de cylindre sera placé
 * @param sect l'adresse de l'endroit ou le numéro de secteur sera placé
 */
void to_physical(int vol, int bloc, int *cyl, int *sect);

void read_bloc(const unsigned int vol, const unsigned int bloc, unsigned char* buffer);

void write_bloc(const unsigned int vol, const unsigned int bloc, const unsigned char* buffer);

void format_vol(const unsigned int vol);

unsigned int new_bloc();

void free_bloc(unsigned int bloc);

void read_blocn(unsigned int vol, unsigned int bloc, unsigned char *buffer, unsigned int bufsize);

void write_blocn(unsigned int vol, unsigned int bloc, unsigned char *buffer, unsigned int bufsize);

void load_super(unsigned int vol);

void save_super();

void init_super(unsigned int vol, char* name);

void read_inode(int bloc, inode_t *inode);

void write_inode(int bloc, inode_t *inode);

int create_inode(enum type_t type);

void delete_inode(int bloc);

#endif
