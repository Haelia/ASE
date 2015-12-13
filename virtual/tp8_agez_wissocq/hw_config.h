#ifndef _HW_CONFIG_H_
#define _HW_CONFIG_H_

#define HARDWARE_INI	"hardware.ini"

#define ENABLE_HDA 1
#define CMD_REG 0x3F6
#define HDA_DATAREGS 0x110
#define HDA_IRQ 14

#define HDA_FILENAME "vdiskA.bin"
#define HDA_SECTORSIZE 256
#define HDA_MAXCYLINDER 16
#define HDA_MAXSECTOR 16


#define N 254
#define PM_PAGES 256
#define VM_PAGES 4096
#define PAGE_SIZE 4*1024
#define PM_SIZE PM_PAGES*PAGE_SIZE
#define VM_SIZE VM_PAGES*PAGE_SIZE
#define MMU_ENABLE	1
#define MMU_IRQ	13
#define MMU_CMD	0x66
#define MMU_FAULT_ADDR	0xCD
#define TLB_ADD_ENTRY	0xCE
#define TLB_DEL_ENTRY	0xDE
#define TLB_SIZE	32
#define TLB_ENTRIES	0x800

#endif
