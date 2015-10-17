#include <stdlib.h>
#include <stdio.h>
#include "hardware.h"
#include "hw_config.h"
#include "drive.h"

void seek (unsigned int cylinder, unsigned int sector) {
  _out(HDA_DATAREGS, (cylinder >> 8) & 0xFF);
  _out(HDA_DATAREGS + 1, (cylinder) & 0xFF);
  _out(HDA_DATAREGS + 2, (sector >> 8) & 0xFF);
  _out(HDA_DATAREGS + 3, (sector) & 0xFF);
  _out(CMD_REG,CMD_SEEK);
  return;
}

void read_sector(unsigned int cylinder,unsigned int sector, unsigned char *buffer) {
  int i;
  seek(cylinder, sector);
  _sleep(HDA_IRQ);
  _out(HDA_DATAREGS,0);
  _out(HDA_DATAREGS + 1,1);
  _out(CMD_REG,CMD_READ);
  _sleep(HDA_IRQ);
  for (i=0; i<HDA_SECTORSIZE ;i++) 
    buffer[i]=MASTERBUFFER[i];
  return;
}

void write_sector(unsigned int cylinder, unsigned int sector, const unsigned char *buffer) {
  int i;
  seek(cylinder, sector);
  _sleep(HDA_IRQ);
  for (i=0; i<HDA_SECTORSIZE ; i++) {
    MASTERBUFFER[i] = buffer[i];
  }
  _out(HDA_DATAREGS,0);
  _out(HDA_DATAREGS + 1,1);
  _out(CMD_REG, CMD_WRITE);
  _sleep(HDA_IRQ);
  return;
}

void format_sector(unsigned int cylinder,unsigned int sector, unsigned int nsector, unsigned int value) {
  seek(cylinder, sector);
  _sleep(HDA_IRQ);
  _out(HDA_DATAREGS, nsector);
  _out(HDA_DATAREGS + 1, (nsector >> 8) & 0xFF);
  _out(HDA_DATAREGS + 2,value);
  _out(HDA_DATAREGS + 3,value);
  _out(HDA_DATAREGS + 4,value);
  _out(HDA_DATAREGS + 5,value);
  _out(CMD_REG, CMD_FORMAT);
  _sleep(HDA_IRQ);
  return;
}
