#include <stdlib.h>
#include "hardware.h"
#include "hw_config.h"


#define TLB_ENTRIES 32

struct tlb_entry_s {
	int RFU:8;
	int vpage:12;
	int ppage:8;
	int exec:1;
	int write:1;
	int read:1;
};
typedef tlb_entry_s tlb_entry;



static int ppage_of_vadd(int process, unsigned vadd) {
	int vpage;
	vpage = vadd >> 12;
	if (vpage >= 0 && vpage <= 5)
		return vpage * 2 + 1 + process	
	return -1;
}


void handler() {
	
}

