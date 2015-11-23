#ifndef _HW_CONFIG_H_
#define _HW_CONFIG_H_

#define HARDWARE_INI	"hardware.ini"




/************************************/
/*** Configuration disque maitre  ***/
/************************************/

#define ENABLE_HDA				1
#define CMD_REG 				0x3F6
#define HDA_DATAREGS 	        0x110
#define HDA_IRQ 				14

#define HDA_FILENAME 	        "vdiskA.bin"
#define HDA_SECTORSIZE 	        256
#define HDA_MAXSECTOR	        16
#define HDA_MAXCYLINDER	        16


/****************************************/
/*** Configuration memoire virtuelle  ***/
/****************************************/

#define MMU_IRQ                 13
#define MMU_FAULT_ADD           0xCD

#endif
