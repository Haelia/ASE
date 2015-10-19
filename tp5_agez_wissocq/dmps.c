#include <stdio.h>
#include <stdlib.h>
#include "drive.h"
#include "hardware.h"
#include "hw_config.h"
#include <assert.h>

#define BUFSIZE 256

//Fonction permettant de remplacer les interruptions
static void nothing(){
  return;
}

void toHexa(unsigned char * buffer) {
  int i,j;
  for (i = 0; i < 16; i++) {
    printf("%03d :",i*16);
    for (j = 0; j < 16; j++) {
      printf(" %02x",buffer[i*16+j]);
    }
    printf("\n");
  }
  return;
}


int main (int argc, char ** argv) {
  if(argc !=3){
     printf("Format necessite 2 arguments: le cylindre et le secteur");
    exit(EXIT_FAILURE);
  }
  int i;
  unsigned int cylinder, sector;
  unsigned char buffer[BUFSIZE];
  cylinder = atoi(argv[1]);
  sector = atoi(argv[2]);
  assert(init_hardware(HARDWARE_INI));
  for(i = 0; i < 15; i++)
    IRQVECTOR[i] = nothing;
  read_sector(cylinder,sector,buffer);
  toHexa(buffer);
  exit(EXIT_SUCCESS);
}
