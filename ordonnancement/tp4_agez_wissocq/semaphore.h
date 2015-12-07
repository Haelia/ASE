#ifndef SEMH
#define SEMH 
#include <stdlib.h>
#include <stdio.h>
#include "ordonnancement.h"

struct sem_s {
    int tokens;
    ctxt *context;
};

typedef struct sem_s semt;

void sem_init(semt *sem, unsigned int val);
void sem_down(semt *sem);
void sem_up(semt *sem);

#endif
