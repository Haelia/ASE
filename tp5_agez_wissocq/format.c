#include <stdio.h>
#include <stdlib.h>
#include "drive.h"
#include "hardware.h"
#include "hw_config.h"
#include <assert.h>

#define BUFSIZE 256

/* Fonction permettant de remplacer les interruptions */
static void nothing(){
  return;
}


int main (int argc, char ** argv) {
    int i;
  unsigned int cylinder, sector,nsector,value;
  if(argc !=5){
     printf("Format necessite 4 arguments: le cylindre, le secteur, le nombre de secteur et la nouvelle valeur");
    exit(EXIT_FAILURE);
  }
  
  cylinder = atoi(argv[1]);
  sector = atoi(argv[2]);
  nsector=atoi(argv[3]);
  value=atoi(argv[4]);
  
  
  assert(init_hardware(HARDWARE_INI));
  for(i = 0; i < 15; i++)
    IRQVECTOR[i] = nothing;
  format_sector(cylinder,sector,nsector,value);
  exit(EXIT_SUCCESS);
}
